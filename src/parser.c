#include "parser.h"

#include <buf/buf.h>
#include <stdlib.h>

#include "binary_expression_syntax.h"
#include "lexer.h"
#include "literal_expression_syntax.h"

typedef BUF(SyntaxToken*) SyntaxTokenBuf;

struct Parser {
    SyntaxTokenBuf tokens;
    size_t position;
};

static SyntaxToken* peek(const Parser* parser, size_t offset);
static SyntaxToken* current(const Parser* parser);
static SyntaxToken* next_token(Parser* parser);
static SyntaxToken* match_token(Parser* parser, SyntaxKind kind);
static ExpressionSyntax* parse_primary_expression(Parser* parser);

Parser* parser_new(str text) {
    Lexer* lexer = lexer_new(text);
    SyntaxTokenBuf tokens = BUF_NEW;

    while (true) {
        SyntaxToken* token = lexer_next_token(lexer);
        if (token->kind != SYNTAX_KIND_WHITESPACE_TOKEN &&
            token->kind != SYNTAX_KIND_BAD_TOKEN) {
            BUF_PUSH(&tokens, token);
        } else {
            syntax_token_free(token);
            token = NULL;
        }
        if (token != NULL && token->kind == SYNTAX_KIND_END_OF_FILE_TOKEN) {
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

ExpressionSyntax* parser_parse(Parser* parser) {
    ExpressionSyntax* left = parse_primary_expression(parser);
    while (current(parser)->kind == SYNTAX_KIND_PLUS_TOKEN ||
           current(parser)->kind == SYNTAX_KIND_MINUS_TOKEN) {
        SyntaxToken* operator_token = syntax_token_dup(next_token(parser));
        ExpressionSyntax* right = parse_primary_expression(parser);
        left = binary_expression_syntax_new(left, operator_token, right);
    }
    return left;
}

static SyntaxToken* peek(const Parser* parser, size_t offset) {
    size_t index = parser->position + offset;
    if (index >= parser->tokens.len) {
        return BUF_LAST(parser->tokens);
    }
    return parser->tokens.ptr[index];
}

static SyntaxToken* current(const Parser* parser) {
    return peek(parser, 0);
}

static SyntaxToken* next_token(Parser* parser) {
    SyntaxToken* curr = current(parser);
    parser->position++;
    return curr;
}

static SyntaxToken* match_token(Parser* parser, SyntaxKind kind) {
    if (current(parser)->kind == kind) {
        return next_token(parser);
    }
    return syntax_token_new(kind, current(parser)->position, str_null, NULL);
}

static ExpressionSyntax* parse_primary_expression(Parser* parser) {
    SyntaxToken* number_token =
            syntax_token_dup(match_token(parser, SYNTAX_KIND_NUMBER_TOKEN));
    return literal_expression_syntax_new(number_token);
}
