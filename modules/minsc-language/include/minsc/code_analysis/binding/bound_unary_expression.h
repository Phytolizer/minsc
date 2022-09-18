#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_EXPRESSION_H_

#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/binding/bound_unary_operator.h"
#include "minsc/runtime/object.h"

typedef struct BoundUnaryExpression {
    BoundExpression base;
    const BoundUnaryOperator* op;
    BoundExpression* operand;
} BoundUnaryExpression;

BoundExpression*
bound_unary_expression_new(const BoundUnaryOperator* op, BoundExpression* operand);
void bound_unary_expression_free(BoundUnaryExpression* expression);

ObjectType bound_unary_expression_type(const BoundUnaryExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_EXPRESSION_H_
