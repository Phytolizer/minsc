#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_VARIABLE_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_VARIABLE_EXPRESSION_H_

#include "minsc/code_analysis/binding/bound_expression.h"

typedef struct BoundVariableExpression {
    BoundExpression base;
    str name;
    ObjectType type;
} BoundVariableExpression;

BoundExpression* bound_variable_expression_new(str name, ObjectType type);
void bound_variable_expression_free(BoundVariableExpression* expression);

ObjectType bound_variable_expression_type(const BoundVariableExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_VARIABLE_EXPRESSION_H_
