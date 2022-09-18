#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_EXPRESSION_H_

#include "minsc/code_analysis/binding/bound_binary_operator.h"
#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/runtime/object.h"

typedef struct BoundBinaryExpression {
    BoundExpression base;
    const BoundBinaryOperator* op;
    BoundExpression* left;
    BoundExpression* right;
} BoundBinaryExpression;

BoundExpression* bound_binary_expression_new(
    const BoundBinaryOperator* op,
    BoundExpression* left,
    BoundExpression* right
);
void bound_binary_expression_free(BoundBinaryExpression* expression);

ObjectType bound_binary_expression_type(const BoundBinaryExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_EXPRESSION_H_
