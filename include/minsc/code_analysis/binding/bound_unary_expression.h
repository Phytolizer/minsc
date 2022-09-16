#ifndef MINSC_BOUND_UNARY_EXPRESSION_H_
#define MINSC_BOUND_UNARY_EXPRESSION_H_

#include "bound_expression.h"
#include "minsc/runtime/object.h"

typedef enum BoundUnaryOperatorKind {
#define X(x) BOUND_UNARY_OPERATOR_KIND_##x,
#include "bound_unary_operator_kind.inc"
#undef X
} BoundUnaryOperatorKind;

typedef struct BoundUnaryExpression {
    BoundExpression base;
    BoundUnaryOperatorKind operator_kind;
    BoundExpression* operand;
} BoundUnaryExpression;

extern BoundExpression* bound_unary_expression_new(
    BoundUnaryOperatorKind operator_kind,
    BoundExpression* operand
);
extern void bound_unary_expression_free(BoundUnaryExpression* expression);

extern ObjectType bound_unary_expression_type(
    const BoundUnaryExpression* expression
);

#endif  // MINSC_BOUND_UNARY_EXPRESSION_H_
