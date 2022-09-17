#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_EXPRESSION_H_

#include "bound_binary_operator.h"
#include "bound_expression.h"

typedef struct BoundBinaryExpression {
    BoundExpression base;
    const BoundBinaryOperator* op;
    BoundExpression* left;
    BoundExpression* right;
} BoundBinaryExpression;

extern BoundExpression* bound_binary_expression_new(
    const BoundBinaryOperator* op,
    BoundExpression* left,
    BoundExpression* right
);
extern void bound_binary_expression_free(BoundBinaryExpression* expression);

extern ObjectType bound_binary_expression_type(const BoundBinaryExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_EXPRESSION_H_
