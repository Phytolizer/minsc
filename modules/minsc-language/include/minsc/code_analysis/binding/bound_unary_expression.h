#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_EXPRESSION_H_

#include "bound_expression.h"
#include "bound_unary_operator.h"
#include "minsc/runtime/object.h"

typedef struct BoundUnaryExpression {
    BoundExpression base;
    const BoundUnaryOperator* op;
    BoundExpression* operand;
} BoundUnaryExpression;

extern BoundExpression*
bound_unary_expression_new(const BoundUnaryOperator* op, BoundExpression* operand);
extern void bound_unary_expression_free(BoundUnaryExpression* expression);

extern ObjectType bound_unary_expression_type(const BoundUnaryExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_EXPRESSION_H_
