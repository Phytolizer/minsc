#include "minsc/code_analysis/binding/bound_unary_expression.h"

#include <stdlib.h>

#include "minsc/support/minsc_assert.h"

extern BoundExpression*
bound_unary_expression_new(const BoundUnaryOperator* op, BoundExpression* operand) {
    BoundUnaryExpression* expression = malloc(sizeof(BoundUnaryExpression));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = BOUND_NODE_TYPE_EXPRESSION;
    expression->base.type = BOUND_EXPRESSION_TYPE_UNARY;
    expression->op = op;
    expression->operand = operand;
    return (BoundExpression*)expression;
}

extern void bound_unary_expression_free(BoundUnaryExpression* expression) {
    bound_expression_free(expression->operand);
    free(expression);
}

extern ObjectType bound_unary_expression_type(const BoundUnaryExpression* expression) {
    return expression->op->result_type;
}
