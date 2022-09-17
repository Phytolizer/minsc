#ifndef MINSC_UNARY_EXPRESSION_SYNTAX_H_
#define MINSC_UNARY_EXPRESSION_SYNTAX_H_

#include "expression_syntax.h"
#include "syntax_token.h"

typedef struct UnaryExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken* operator_token;
    ExpressionSyntax* operand;
} UnaryExpressionSyntax;

ExpressionSyntax*
unary_expression_syntax_new(SyntaxToken* operator_token, ExpressionSyntax* operand);
void unary_expression_syntax_free(UnaryExpressionSyntax* expression);

SyntaxNodeChildren unary_expression_syntax_children(const UnaryExpressionSyntax* expression);

#endif  // MINSC_UNARY_EXPRESSION_SYNTAX_H_
