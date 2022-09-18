#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_ASSIGNMENT_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_ASSIGNMENT_EXPRESSION_H_

#include "minsc/code_analysis/binding/bound_expression.h"

typedef struct BoundAssignmentExpression {
    BoundExpression base;
    str name;
    BoundExpression* expression;
} BoundAssignmentExpression;

BoundExpression* bound_assignment_expression_new(str name, BoundExpression* expression);
void bound_assignment_expression_free(BoundAssignmentExpression* expression);

ObjectType bound_assignment_expression_type(const BoundAssignmentExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_ASSIGNMENT_EXPRESSION_H_
