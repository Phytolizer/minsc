#include "lexer.h"

#include <println/println.h>
#include <stddef.h>
#include <stdlib.h>
#include <str/strtox.h>

#include "diagnostic.h"
#include "wrap_ctype.h"

struct Lexer {
    str source;
    size_t position;
    DiagnosticBuf diagnostics;
};

static char current(const Lexer* lexer);

Lexer* lexer_new(str source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->position = 0;
    lexer->diagnostics = (DiagnosticBuf)BUF_NEW;
    return lexer;
}

void lexer_free(Lexer* lexer) {
    diagnostic_buf_free(lexer->diagnostics);
    str_free(lexer->source);
    free(lexer);
}

DiagnosticBuf lexer_take_diagnostics(Lexer* lexer) {
    DiagnosticBuf diagnostics = lexer->diagnostics;
    lexer->diagnostics = (DiagnosticBuf)BUF_NEW;
    return diagnostics;
}

SyntaxToken* lexer_next_token(Lexer* lexer) {
    size_t start = lexer->position;
    char c = current(lexer);

    if (wrap_isdigit(c)) {
        while (wrap_isdigit(current(lexer))) {
            lexer->position++;
        }

        str text = str_ref_chars(lexer->source.ptr + start,
                                 lexer->position - start);
        Str2U64Result result = str2u64(text, 10);
        if (result.err) {
            BUF_PUSH(
                    &lexer->diagnostics,
                    str_printf("ERROR: The number %s is too large.", text.ptr));
        }
        str owned_text = str_null;
        str_cpy(&owned_text, text);
        Object* value = object_new_u64(result.value);
        return syntax_token_new(
                SYNTAX_KIND_NUMBER_TOKEN, start, owned_text, value);
    }

    if (wrap_isspace(c)) {
        while (wrap_isspace(current(lexer))) {
            lexer->position++;
        }

        str text = str_ref_chars(lexer->source.ptr + start,
                                 lexer->position - start);
        str owned_text = str_null;
        str_cpy(&owned_text, text);
        return syntax_token_new(
                SYNTAX_KIND_WHITESPACE_TOKEN, start, owned_text, NULL);
    }

    switch (c) {
        case '+':
            lexer->position++;
            return syntax_token_new(
                    SYNTAX_KIND_PLUS_TOKEN, start, str_lit("+"), NULL);
        case '-':
            lexer->position++;
            return syntax_token_new(
                    SYNTAX_KIND_MINUS_TOKEN, start, str_lit("-"), NULL);
        case '*':
            lexer->position++;
            return syntax_token_new(
                    SYNTAX_KIND_STAR_TOKEN, start, str_lit("*"), NULL);
        case '/':
            lexer->position++;
            return syntax_token_new(
                    SYNTAX_KIND_SLASH_TOKEN, start, str_lit("/"), NULL);
        case '(':
            lexer->position++;
            return syntax_token_new(SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN,
                                    start,
                                    str_lit("("),
                                    NULL);
        case ')':
            lexer->position++;
            return syntax_token_new(SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN,
                                    start,
                                    str_lit(")"),
                                    NULL);
        case '\0':
            return syntax_token_new(
                    SYNTAX_KIND_END_OF_FILE_TOKEN, start, str_lit(""), NULL);
        default: {
            char text[] = {current(lexer)};
            str owned_text = str_null;
            str_cpy(&owned_text, str_ref_chars(text, 1));
            BUF_PUSH(&lexer->diagnostics,
                     str_printf("ERROR: Unexpected character '%c'.",
                                current(lexer)));
            lexer->position++;
            return syntax_token_new(
                    SYNTAX_KIND_BAD_TOKEN, start, owned_text, NULL);
        }
    }
}

static char current(const Lexer* lexer) {
    if (lexer->position >= lexer->source.len) {
        return '\0';
    }
    return lexer->source.ptr[lexer->position];
}
