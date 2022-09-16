#ifndef MINSC_BINARY_EXPRESSION_SYNTAX_H_
#define MINSC_BINARY_EXPRESSION_SYNTAX_H_

#include "expression_syntax.h"
#include "syntax_node.h"
#include "syntax_token.h"

typedef struct BinaryExpressionSyntax {
    ExpressionSyntax base;
    ExpressionSyntax* left;
    SyntaxToken* operator_token;
    ExpressionSyntax* right;
} BinaryExpressionSyntax;

ExpressionSyntax* binary_expression_syntax_new(
    ExpressionSyntax* left,
    SyntaxToken* operator_token,
    ExpressionSyntax* right
);
void binary_expression_syntax_free(BinaryExpressionSyntax* expression);

SyntaxNodeChildren binary_expression_syntax_children(
    const BinaryExpressionSyntax* expression
);

#endif  // MINSC_BINARY_EXPRESSION_SYNTAX_H_
