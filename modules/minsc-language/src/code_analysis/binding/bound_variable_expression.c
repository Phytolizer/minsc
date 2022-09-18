#include "minsc/code_analysis/binding/bound_variable_expression.h"

#include <stdlib.h>

#include "minsc/support/minsc_assert.h"

BoundExpression* bound_variable_expression_new(str name, ObjectType type) {
    BoundVariableExpression* expression = malloc(sizeof(BoundVariableExpression));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = BOUND_NODE_TYPE_EXPRESSION;
    expression->base.type = BOUND_EXPRESSION_TYPE_VARIABLE;
    expression->name = name;
    expression->type = type;
    return (BoundExpression*)expression;
}

void bound_variable_expression_free(BoundVariableExpression* expression) {
    str_free(expression->name);
    free(expression);
}

ObjectType bound_variable_expression_type(const BoundVariableExpression* expression) {
    return expression->type;
}
