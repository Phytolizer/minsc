#include "minsc/code_analysis/syntax/parser.h"

#include <buf/buf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <str/str.h>

#include "minsc/code_analysis/syntax/binary_expression_syntax.h"
#include "minsc/code_analysis/syntax/diagnostic.h"
#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/lexer.h"
#include "minsc/code_analysis/syntax/literal_expression_syntax.h"
#include "minsc/code_analysis/syntax/parenthesized_expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_facts.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/code_analysis/syntax/syntax_tree.h"
#include "minsc/code_analysis/syntax/unary_expression_syntax.h"
#include "minsc/runtime/object.h"

typedef BUF(SyntaxToken*) SyntaxTokenBuf;

struct Parser {
    SyntaxTokenBuf tokens;
    size_t position;
    DiagnosticBuf diagnostics;
};

static SyntaxToken* peek(const Parser* parser, size_t offset);
static SyntaxToken* current(const Parser* parser);
static SyntaxToken* next_token(Parser* parser);
static SyntaxToken* match_token(Parser* parser, SyntaxKind kind);
static ExpressionSyntax* parse_expression(Parser* parser, size_t parent_precedence);
static ExpressionSyntax* parse_primary_expression(Parser* parser);

Parser* parser_new(str text) {
    Lexer* lexer = lexer_new(text);
    SyntaxTokenBuf tokens = BUF_NEW;

    while (true) {
        SyntaxToken* token = lexer_next_token(lexer);
        if (token->kind != SYNTAX_KIND_WHITESPACE_TOKEN && token->kind != SYNTAX_KIND_BAD_TOKEN) {
            BUF_PUSH(&tokens, token);
        } else {
            syntax_token_free(token);
            token = NULL;
        }
        if (token != NULL && token->kind == SYNTAX_KIND_END_OF_FILE_TOKEN) {
            break;
        }
    }

    DiagnosticBuf diagnostics = lexer_take_diagnostics(lexer);
    lexer_free(lexer);

    Parser* parser = malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->position = 0;
    parser->diagnostics = diagnostics;
    return parser;
}

void parser_free(Parser* parser) {
    for (size_t i = 0; i < parser->tokens.len; i++) {
        syntax_token_free(parser->tokens.ptr[i]);
    }
    BUF_FREE(parser->tokens);
    diagnostic_buf_free(parser->diagnostics);
    free(parser);
}

SyntaxTree* parser_parse(Parser* parser) {
    ExpressionSyntax* root = parse_expression(parser, 0);
    SyntaxToken* end_of_file_token =
        syntax_token_dup(match_token(parser, SYNTAX_KIND_END_OF_FILE_TOKEN));
    DiagnosticBuf diagnostics = parser_take_diagnostics(parser);
    return syntax_tree_new(diagnostics, root, end_of_file_token);
}

DiagnosticBuf parser_take_diagnostics(Parser* parser) {
    DiagnosticBuf diagnostics = parser->diagnostics;
    parser->diagnostics = (DiagnosticBuf)BUF_NEW;
    return diagnostics;
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

    BUF_PUSH(
        &parser->diagnostics,
        str_printf(
            "ERROR: Unexpected token <" str_fmt ">, expected <" str_fmt ">",
            str_arg(syntax_kind_string(current(parser)->kind)),
            str_arg(syntax_kind_string(kind))
        )
    );
    return syntax_token_new_manufactured(kind, current(parser)->position, str_null, NULL);
}

static ExpressionSyntax* parse_expression(Parser* parser, size_t parent_precedence) {
    ExpressionSyntax* left;
    size_t unary_precedence = unary_operator_precedence(current(parser)->kind);
    if (unary_precedence != 0 && unary_precedence >= parent_precedence) {
        SyntaxToken* operator_token = syntax_token_dup(next_token(parser));
        ExpressionSyntax* operand = parse_expression(parser, unary_precedence);
        left = unary_expression_syntax_new(operator_token, operand);
    } else {
        left = parse_primary_expression(parser);
    }

    while (true) {
        size_t precedence = binary_operator_precedence(current(parser)->kind);
        if (precedence == 0 || precedence <= parent_precedence) {
            break;
        }

        SyntaxToken* operator_token = syntax_token_dup(next_token(parser));
        ExpressionSyntax* right = parse_expression(parser, precedence);
        left = binary_expression_syntax_new(left, operator_token, right);
    }

    return left;
}

static ExpressionSyntax* parse_primary_expression(Parser* parser) {
    ExpressionSyntax* result;

    switch (current(parser)->kind) {
        case SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN: {
            SyntaxToken* left = syntax_token_dup(next_token(parser));
            ExpressionSyntax* expression = parse_expression(parser, 0);
            SyntaxToken* right =
                syntax_token_dup(match_token(parser, SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN));
            result = parenthesized_expression_syntax_new(left, expression, right);
        } break;
        case SYNTAX_KIND_TRUE_KEYWORD:
        case SYNTAX_KIND_FALSE_KEYWORD: {
            bool value = current(parser)->kind == SYNTAX_KIND_TRUE_KEYWORD;
            SyntaxToken* keyword_token = syntax_token_dup(next_token(parser));
            result = literal_expression_syntax_new(keyword_token, object_new_bool(value));
        } break;
        default: {
            SyntaxToken* number_token =
                syntax_token_dup(match_token(parser, SYNTAX_KIND_NUMBER_TOKEN));
            result = literal_expression_syntax_new(number_token, NULL);
        } break;
    }

    return result;
}
