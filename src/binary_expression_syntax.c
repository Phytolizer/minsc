#include "binary_expression_syntax.h"

#include <stdlib.h>

ExpressionSyntax* binary_expression_syntax_new(ExpressionSyntax* left,
                                               SyntaxToken operator_token,
                                               ExpressionSyntax* right) {
    BinaryExpressionSyntax* expression = malloc(sizeof(BinaryExpressionSyntax));
    expression->base.type = EXPRESSION_SYNTAX_TYPE_BINARY;
    expression->left = left;
    expression->operator_token = operator_token;
    expression->right = right;
    return (ExpressionSyntax*)expression;
}

void binary_expression_syntax_free(BinaryExpressionSyntax* expression) {
    expression_syntax_free(expression->left);
    syntax_token_free(expression->operator_token);
    expression_syntax_free(expression->right);
    free(expression);
}
