#include "minsc/code_analysis/binding/bound_assignment_expression.h"

#include <stdlib.h>

#include "minsc/support/minsc_assert.h"

BoundExpression* bound_assignment_expression_new(str name, BoundExpression* expression) {
    BoundAssignmentExpression* assignment_expression = malloc(sizeof(BoundAssignmentExpression));
    MINSC_ASSERT(assignment_expression != NULL);
    assignment_expression->base.base.type = BOUND_NODE_TYPE_EXPRESSION;
    assignment_expression->base.type = BOUND_EXPRESSION_TYPE_ASSIGNMENT;
    assignment_expression->name = name;
    assignment_expression->expression = expression;
    return (BoundExpression*)assignment_expression;
}

void bound_assignment_expression_free(BoundAssignmentExpression* expression) {
    str_free(expression->name);
    bound_expression_free(expression->expression);
    free(expression);
}

ObjectType bound_assignment_expression_type(const BoundAssignmentExpression* expression) {
    return bound_expression_type(expression->expression);
}
