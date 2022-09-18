#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_EXPRESSION_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_EXPRESSION_H_

#include "minsc/code_analysis/binding/bound_node.h"
#include "minsc/runtime/object.h"

typedef enum {
#define X(x) BOUND_EXPRESSION_TYPE_##x,
#include "minsc/code_analysis/binding/bound_expression_type.inc"
#undef X
} BoundExpressionType;

typedef struct BoundExpression {
    BoundNode base;
    BoundExpressionType type;
} BoundExpression;

extern void bound_expression_free(BoundExpression* expression);

extern BoundNodeKind bound_expression_kind(const BoundExpression* expression);
extern ObjectType bound_expression_type(const BoundExpression* expression);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_EXPRESSION_H_
