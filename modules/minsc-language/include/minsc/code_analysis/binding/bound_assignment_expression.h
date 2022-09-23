#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_ASSIGNMENT_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_ASSIGNMENT_EXPRESSION_H_

#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/variable_symbol.h"
#include "minsc/runtime/object.h"

typedef struct BoundAssignmentExpression {
    BoundExpression base;
    VariableSymbol variable;
    BoundExpression* expression;
} BoundAssignmentExpression;

BoundExpression*
bound_assignment_expression_new(VariableSymbol variable, BoundExpression* expression);
void bound_assignment_expression_free(BoundAssignmentExpression* expression);

ObjectType bound_assignment_expression_type(const BoundAssignmentExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_ASSIGNMENT_EXPRESSION_H_
