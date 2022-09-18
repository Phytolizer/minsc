#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_OPERATOR_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_OPERATOR_H_

#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/runtime/object.h"

typedef enum BoundUnaryOperatorKind {
#define X(x) BOUND_UNARY_OPERATOR_KIND_##x,
#include "minsc/code_analysis/binding/bound_unary_operator_kind.inc"
#undef X
} BoundUnaryOperatorKind;

typedef struct BoundUnaryOperator {
    SyntaxKind syntax_kind;
    BoundUnaryOperatorKind kind;
    ObjectType operand_type;
    ObjectType result_type;
} BoundUnaryOperator;

const BoundUnaryOperator* bind_unary_operator(SyntaxKind syntax_kind, ObjectType operand_type);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_UNARY_OPERATOR_H_
