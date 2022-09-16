#include "minsc/code_analysis/binding/bound_binary_expression.h"

#include <stdlib.h>

#include "minsc/support/minsc_assert.h"

extern BoundExpression* bound_binary_expression_new(
        BoundBinaryOperatorKind operator_kind,
        BoundExpression* left,
        BoundExpression* right) {
    BoundBinaryExpression* expression = malloc(sizeof(BoundBinaryExpression));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = BOUND_NODE_TYPE_EXPRESSION;
    expression->base.type = BOUND_EXPRESSION_TYPE_BINARY;
    expression->operator_kind = operator_kind;
    expression->left = left;
    expression->right = right;
    return (BoundExpression*)expression;
}

extern void bound_binary_expression_free(BoundBinaryExpression* expression) {
    bound_expression_free(expression->left);
    bound_expression_free(expression->right);
    free(expression);
}

extern ObjectType bound_binary_expression_type(
        const BoundBinaryExpression* expression) {
    return bound_expression_type(expression->left);
}