#include "minsc/code_analysis/syntax/lexer.h"

#include <stddef.h>
#include <stdlib.h>
#include <str/str.h>
#include <str/strtox.h>

#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/code_analysis/syntax/syntax_facts.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/code_analysis/text_span.h"
#include "minsc/runtime/object.h"
#include "minsc/support/minsc_assert.h"
#include "minsc/support/wrap_ctype.h"

struct Lexer {
    str source;
    size_t position;
    DiagnosticBag* diagnostics;
    SyntaxKind kind;
    size_t start;
    Object* value;
};

static char peek(const Lexer* lexer, size_t offset);
static char current(const Lexer* lexer);
static str ref_text(const Lexer* lexer);
static void read_number_token(Lexer* lexer);
static void read_whitespace_token(Lexer* lexer);
static void read_identifier_or_keyword(Lexer* lexer);

Lexer* lexer_new(str source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    MINSC_ASSERT(lexer != NULL);
    lexer->source = source;
    lexer->position = 0;
    lexer->diagnostics = diagnostic_bag_new();
    lexer->kind = SYNTAX_KIND_BAD_TOKEN;
    lexer->start = 0;
    lexer->value = NULL;
    return lexer;
}

void lexer_free(Lexer* lexer) {
    diagnostic_bag_free(lexer->diagnostics);
    str_free(lexer->source);
    free(lexer);
}

DiagnosticBag* lexer_take_diagnostics(Lexer* lexer) {
    DiagnosticBag* diagnostics = lexer->diagnostics;
    lexer->diagnostics = NULL;
    return diagnostics;
}

SyntaxToken* lexer_next_token(Lexer* lexer) {
    lexer->start = lexer->position;
    lexer->kind = SYNTAX_KIND_BAD_TOKEN;
    lexer->value = NULL;

    char c = current(lexer);
    switch (c) {
        case '+':
            lexer->position++;
            lexer->kind = SYNTAX_KIND_PLUS_TOKEN;
            break;
        case '-':
            lexer->position++;
            lexer->kind = SYNTAX_KIND_MINUS_TOKEN;
            break;
        case '*':
            lexer->position++;
            lexer->kind = SYNTAX_KIND_STAR_TOKEN;
            break;
        case '/':
            lexer->position++;
            lexer->kind = SYNTAX_KIND_SLASH_TOKEN;
            break;
        case '(':
            lexer->position++;
            lexer->kind = SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN;
            break;
        case ')':
            lexer->position++;
            lexer->kind = SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN;
            break;
        case '!':
            if (peek(lexer, 1) == '=') {
                lexer->position += 2;
                lexer->kind = SYNTAX_KIND_BANG_EQUALS_TOKEN;
            } else {
                lexer->position++;
                lexer->kind = SYNTAX_KIND_BANG_TOKEN;
            }
            break;
        case '&':
            if (peek(lexer, 1) == '&') {
                lexer->position += 2;
                lexer->kind = SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN;
            } else {
                lexer->position++;
            }
            break;
        case '|':
            if (peek(lexer, 1) == '|') {
                lexer->position += 2;
                lexer->kind = SYNTAX_KIND_PIPE_PIPE_TOKEN;
            } else {
                lexer->position++;
            }
            break;
        case '=':
            if (peek(lexer, 1) == '=') {
                lexer->position += 2;
                lexer->kind = SYNTAX_KIND_EQUALS_EQUALS_TOKEN;
            } else {
                lexer->position++;
                lexer->kind = SYNTAX_KIND_EQUALS_TOKEN;
            }
            break;
        case '\0':
            lexer->kind = SYNTAX_KIND_END_OF_FILE_TOKEN;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            read_number_token(lexer);
            break;
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            read_whitespace_token(lexer);
            break;
        default:
            if (wrap_isdigit(c)) {
                read_number_token(lexer);
            } else if (wrap_isspace(c)) {
                read_whitespace_token(lexer);
            } else if (wrap_isalpha(c)) {
                read_identifier_or_keyword(lexer);
            } else {
                lexer->position++;
            }
    }

    if (lexer->kind == SYNTAX_KIND_BAD_TOKEN) {
        diagnostic_bag_report_bad_character(lexer->diagnostics, lexer->start, c);
    }

    str text = syntax_facts_get_text(lexer->kind);
    if (text.len == 0) {
        text = str_dup(ref_text(lexer));
    }
    return syntax_token_new(lexer->kind, lexer->start, text, lexer->value);
}

static char peek(const Lexer* lexer, size_t offset) {
    size_t position = lexer->position + offset;
    if (position >= str_len(lexer->source)) {
        return '\0';
    }
    return lexer->source.ptr[position];
}

static char current(const Lexer* lexer) {
    return peek(lexer, 0);
}

static str ref_text(const Lexer* lexer) {
    return str_ref_chars(lexer->source.ptr + lexer->start, lexer->position - lexer->start);
}

static void read_number_token(Lexer* lexer) {
    while (wrap_isdigit(current(lexer))) {
        lexer->position++;
    }

    str temp_text = ref_text(lexer);
    Str2I64Result result = str2i64(temp_text, 10);
    if (result.err) {
        diagnostic_bag_report_invalid_int64(
            lexer->diagnostics,
            (TextSpan){
                .start = lexer->start,
                .length = lexer->position - lexer->start,
            },
            temp_text
        );
    }
    lexer->value = result.err ? NULL : object_new_i64(result.value);
    lexer->kind = SYNTAX_KIND_NUMBER_TOKEN;
}

static void read_whitespace_token(Lexer* lexer) {
    while (wrap_isspace(current(lexer))) {
        lexer->position++;
    }

    lexer->kind = SYNTAX_KIND_WHITESPACE_TOKEN;
}

static void read_identifier_or_keyword(Lexer* lexer) {
    while (wrap_isalpha(current(lexer))) {
        lexer->position++;
    }

    str temp_text = ref_text(lexer);
    lexer->kind = keyword_kind(temp_text);
}
