#include "lexer.h"

#include <println/println.h>
#include <stddef.h>
#include <stdlib.h>
#include <str/strtox.h>

#include "wrap_ctype.h"

struct Lexer {
    str source;
    size_t position;
};

static char current(const Lexer* lexer);

Lexer* lexer_new(str source) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->position = 0;
    return lexer;
}

void lexer_free(Lexer* lexer) {
    str_free(lexer->source);
    free(lexer);
}

SyntaxToken lexer_next_token(Lexer* lexer) {
    size_t start = lexer->position;
    char c = current(lexer);

    if (wrap_isdigit(c)) {
        while (wrap_isdigit(current(lexer))) {
            lexer->position++;
        }

        str text =
            str_ref_chars(lexer->source.ptr + start, lexer->position - start);
        Str2U64Result result = str2u64(text, 10);
        if (result.err) {
            fprintfln(stderr, "TODO");
            abort();
        }
        str owned_text = str_null;
        str_cpy(&owned_text, text);
        Object* value = object_new_u64(result.value);
        return (SyntaxToken){
            .kind = SYNTAX_KIND_NUMBER_TOKEN,
            .position = start,
            .text = owned_text,
            .value = value,
        };
    }

    if (wrap_isspace(c)) {
        while (wrap_isspace(current(lexer))) {
            lexer->position++;
        }

        str text =
            str_ref_chars(lexer->source.ptr + start, lexer->position - start);
        str owned_text = str_null;
        str_cpy(&owned_text, text);
        return (SyntaxToken){
            .kind = SYNTAX_KIND_WHITESPACE_TOKEN,
            .position = start,
            .text = owned_text,
            .value = NULL,
        };
    }

    switch (c) {
        case '+':
            lexer->position++;
            return (SyntaxToken){
                .kind = SYNTAX_KIND_PLUS_TOKEN,
                .position = start,
                .text = str_lit("+"),
                .value = NULL,
            };
        case '-':
            lexer->position++;
            return (SyntaxToken){
                .kind = SYNTAX_KIND_MINUS_TOKEN,
                .position = start,
                .text = str_lit("-"),
                .value = NULL,
            };
        case '*':
            lexer->position++;
            return (SyntaxToken){
                .kind = SYNTAX_KIND_STAR_TOKEN,
                .position = start,
                .text = str_lit("*"),
                .value = NULL,
            };
        case '/':
            lexer->position++;
            return (SyntaxToken){
                .kind = SYNTAX_KIND_SLASH_TOKEN,
                .position = start,
                .text = str_lit("/"),
                .value = NULL,
            };
        case '(':
            lexer->position++;
            return (SyntaxToken){
                .kind = SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN,
                .position = start,
                .text = str_lit("("),
                .value = NULL,
            };
        case ')':
            lexer->position++;
            return (SyntaxToken){
                .kind = SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN,
                .position = start,
                .text = str_lit(")"),
                .value = NULL,
            };
        case '\0':
            return (SyntaxToken){
                .kind = SYNTAX_KIND_END_OF_FILE_TOKEN,
                .position = start,
                .text = str_lit(""),
                .value = NULL,
            };
        default: {
            char text[] = {current(lexer)};
            str owned_text = str_null;
            str_cpy(&owned_text, str_ref_chars(text, 1));
            lexer->position++;
            return (SyntaxToken){
                .kind = SYNTAX_KIND_BAD_TOKEN,
                .position = start,
                .text = owned_text,
                .value = NULL,
            };
        }
    }
}

static char current(const Lexer* lexer) {
    if (lexer->position >= lexer->source.len) {
        return '\0';
    }
    return lexer->source.ptr[lexer->position];
}
