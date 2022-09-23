#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_VARIABLE_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_VARIABLE_EXPRESSION_H_

#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/variable_symbol.h"
#include "minsc/runtime/object.h"

typedef struct BoundVariableExpression {
    BoundExpression base;
    VariableSymbol variable;
} BoundVariableExpression;

BoundExpression* bound_variable_expression_new(VariableSymbol variable);
void bound_variable_expression_free(BoundVariableExpression* expression);

ObjectType bound_variable_expression_type(const BoundVariableExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_VARIABLE_EXPRESSION_H_
