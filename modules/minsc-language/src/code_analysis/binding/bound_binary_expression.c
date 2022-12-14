#include "minsc/code_analysis/binding/bound_binary_expression.h"

#include <stdlib.h>

#include "minsc/code_analysis/binding/bound_binary_operator.h"
#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/binding/bound_node.h"
#include "minsc/support/minsc_assert.h"

BoundExpression* bound_binary_expression_new(
    const BoundBinaryOperator* op,
    BoundExpression* left,
    BoundExpression* right
) {
    BoundBinaryExpression* expression = malloc(sizeof(BoundBinaryExpression));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = BOUND_NODE_TYPE_EXPRESSION;
    expression->base.type = BOUND_EXPRESSION_TYPE_BINARY;
    expression->op = op;
    expression->left = left;
    expression->right = right;
    return (BoundExpression*)expression;
}

void bound_binary_expression_free(BoundBinaryExpression* expression) {
    bound_expression_free(expression->left);
    bound_expression_free(expression->right);
    free(expression);
}

ObjectType bound_binary_expression_type(const BoundBinaryExpression* expression) {
    return expression->op->result_type;
}
