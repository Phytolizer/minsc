#include "parser.h"

#include <buf/buf.h>
#include <stdlib.h>

#include "lexer.h"

typedef BUF(SyntaxToken) SyntaxTokenBuf;

struct Parser {
    SyntaxTokenBuf tokens;
    size_t position;
};

static SyntaxToken peek(const Parser* parser, size_t offset);
static SyntaxToken current(const Parser* parser);

Parser* parser_new(str text) {
    Lexer* lexer = lexer_new(text);
    SyntaxTokenBuf tokens = BUF_NEW;

    while (true) {
        SyntaxToken token = lexer_next_token(lexer);
        if (token.kind != SYNTAX_KIND_WHITESPACE_TOKEN &&
            token.kind != SYNTAX_KIND_BAD_TOKEN) {
            BUF_PUSH(&tokens, token);
        } else {
            syntax_token_free(token);
        }
        if (token.kind == SYNTAX_KIND_END_OF_FILE_TOKEN) {
            break;
        }
    }

    lexer_free(lexer);

    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->position = 0;
    return parser;
}

void parser_free(Parser* parser) {
    for (size_t i = 0; i < parser->tokens.len; i++) {
        syntax_token_free(parser->tokens.ptr[i]);
    }
    BUF_FREE(parser->tokens);
    free(parser);
}

static SyntaxToken peek(const Parser* parser, size_t offset) {
    size_t index = parser->position + offset;
    if (index >= parser->tokens.len) {
        return BUF_LAST(parser->tokens);
    }
    return parser->tokens.ptr[index];
}

static SyntaxToken current(const Parser* parser) {
    return peek(parser, 0);
}
