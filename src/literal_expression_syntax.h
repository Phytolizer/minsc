#ifndef MINSC_LITERAL_EXPRESSION_SYNTAX_H_
#define MINSC_LITERAL_EXPRESSION_SYNTAX_H_

#include "expression_syntax.h"
#include "syntax_token.h"

typedef struct LiteralExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken literal_token;
} LiteralExpressionSyntax;

ExpressionSyntax* literal_expression_syntax_new(
    SyntaxToken literal_token);
void literal_expression_syntax_free(LiteralExpressionSyntax* expression);

#endif  // MINSC_LITERAL_EXPRESSION_SYNTAX_H_
