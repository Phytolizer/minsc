#ifndef MINSC_BOUND_BINARY_EXPRESSION_H_
#define MINSC_BOUND_BINARY_EXPRESSION_H_

#include "bound_expression.h"

typedef enum {
#define X(x) BOUND_BINARY_OPERATOR_KIND_##x,
#include "bound_binary_operator_kind.inc"
#undef X
} BoundBinaryOperatorKind;

typedef struct BoundBinaryExpression {
    BoundExpression base;
    BoundBinaryOperatorKind operator_kind;
    BoundExpression* left;
    BoundExpression* right;
} BoundBinaryExpression;

extern BoundExpression* bound_binary_expression_new(
    BoundBinaryOperatorKind operator_kind,
    BoundExpression* left,
    BoundExpression* right
);
extern void bound_binary_expression_free(BoundBinaryExpression* expression);

extern ObjectType bound_binary_expression_type(
    const BoundBinaryExpression* expression
);

#endif  // MINSC_BOUND_BINARY_EXPRESSION_H_
