#include "minsc/code_analysis/syntax/lexer.h"

#include <stdbool.h>
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
};

static char peek(const Lexer* lexer, size_t offset);
static char current(const Lexer* lexer);
static str ref_text(const Lexer* lexer, size_t start);

Lexer* lexer_new(str source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    MINSC_ASSERT(lexer != NULL);
    lexer->source = source;
    lexer->position = 0;
    lexer->diagnostics = diagnostic_bag_new();
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
    size_t start = lexer->position;
    char c = current(lexer);
    SyntaxKind kind;
    Object* value = NULL;
    bool c_is_error = false;

    if (wrap_isdigit(c)) {
        while (wrap_isdigit(current(lexer))) {
            lexer->position++;
        }

        str temp_text = ref_text(lexer, start);
        Str2I64Result result = str2i64(temp_text, 10);
        if (result.err) {
            diagnostic_bag_report_invalid_int64(
                lexer->diagnostics,
                (TextSpan){.start = start, .length = lexer->position - start},
                temp_text
            );
        }
        value = result.err ? NULL : object_new_i64(result.value);
        kind = SYNTAX_KIND_NUMBER_TOKEN;
    } else if (wrap_isspace(c)) {
        while (wrap_isspace(current(lexer))) {
            lexer->position++;
        }

        kind = SYNTAX_KIND_WHITESPACE_TOKEN;
    } else if (wrap_isalpha(c)) {
        while (wrap_isalpha(current(lexer))) {
            lexer->position++;
        }

        str temp_text = ref_text(lexer, start);
        kind = keyword_kind(temp_text);
    } else {
        switch (c) {
            case '+':
                lexer->position++;
                kind = SYNTAX_KIND_PLUS_TOKEN;
                break;
            case '-':
                lexer->position++;
                kind = SYNTAX_KIND_MINUS_TOKEN;
                break;
            case '*':
                lexer->position++;
                kind = SYNTAX_KIND_STAR_TOKEN;
                break;
            case '/':
                lexer->position++;
                kind = SYNTAX_KIND_SLASH_TOKEN;
                break;
            case '(':
                lexer->position++;
                kind = SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN;
                break;
            case ')':
                lexer->position++;
                kind = SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN;
                break;
            case '!':
                if (peek(lexer, 1) == '=') {
                    lexer->position += 2;
                    kind = SYNTAX_KIND_BANG_EQUALS_TOKEN;
                } else {
                    lexer->position++;
                    kind = SYNTAX_KIND_BANG_TOKEN;
                }
                break;
            case '&':
                if (peek(lexer, 1) == '&') {
                    lexer->position += 2;
                    kind = SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN;
                } else {
                    lexer->position++;
                    c_is_error = true;
                }
                break;
            case '|':
                if (peek(lexer, 1) == '|') {
                    lexer->position += 2;
                    kind = SYNTAX_KIND_PIPE_PIPE_TOKEN;
                } else {
                    lexer->position++;
                    c_is_error = true;
                }
                break;
            case '=':
                if (peek(lexer, 1) == '=') {
                    lexer->position += 2;
                    kind = SYNTAX_KIND_EQUALS_EQUALS_TOKEN;
                } else {
                    lexer->position++;
                    kind = SYNTAX_KIND_EQUALS_TOKEN;
                }
                break;
            case '\0':
                kind = SYNTAX_KIND_END_OF_FILE_TOKEN;
                break;
            default:
                lexer->position++;
                c_is_error = true;
        }
    }

    if (c_is_error) {
        diagnostic_bag_report_bad_character(lexer->diagnostics, start, c);
        kind = SYNTAX_KIND_BAD_TOKEN;
    }

    str text = str_dup(ref_text(lexer, start));
    return syntax_token_new(kind, start, text, value);
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

static str ref_text(const Lexer* lexer, size_t start) {
    return str_ref_chars(lexer->source.ptr + start, lexer->position - start);
}
