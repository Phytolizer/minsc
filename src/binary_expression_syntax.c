#include "binary_expression_syntax.h"

#include <stdlib.h>

#include "buf/buf.h"
#include "minsc_assert.h"

ExpressionSyntax* binary_expression_syntax_new(ExpressionSyntax* left,
                                               SyntaxToken* operator_token,
                                               ExpressionSyntax* right) {
    BinaryExpressionSyntax* expression = malloc(sizeof(BinaryExpressionSyntax));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = SYNTAX_NODE_TYPE_EXPRESSION;
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

SyntaxNodeChildren binary_expression_syntax_children(
        const BinaryExpressionSyntax* expression) {
    SyntaxNodeChildren children = BUF_NEW;
    BUF_PUSH(&children, (SyntaxNode*)expression->left);
    BUF_PUSH(&children, (SyntaxNode*)expression->operator_token);
    BUF_PUSH(&children, (SyntaxNode*)expression->right);
    return children;
}
