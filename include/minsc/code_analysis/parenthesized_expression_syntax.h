#ifndef MINSC_PARENTHESIZED_EXPRESSION_SYNTAX_H_
#define MINSC_PARENTHESIZED_EXPRESSION_SYNTAX_H_

#include "expression_syntax.h"
#include "syntax_node.h"
#include "syntax_token.h"

typedef struct ParenthesizedExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken* open_parenthesis_token;
    ExpressionSyntax* expression;
    SyntaxToken* close_parenthesis_token;
} ParenthesizedExpressionSyntax;

ExpressionSyntax* parenthesized_expression_syntax_new(
        SyntaxToken* open_parenthesis_token,
        ExpressionSyntax* expression,
        SyntaxToken* close_parenthesis_token);
void parenthesized_expresion_syntax_free(ParenthesizedExpressionSyntax* syntax);

SyntaxNodeChildren parenthesized_expression_syntax_children(
        const ParenthesizedExpressionSyntax* syntax);

#endif  // MINSC_PARENTHESIZED_EXPRESSION_SYNTAX_H_
