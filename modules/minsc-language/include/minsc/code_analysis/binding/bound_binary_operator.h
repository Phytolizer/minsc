#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_OPERATOR_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_OPERATOR_H_

#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/runtime/object.h"

typedef enum {
#define X(x) BOUND_BINARY_OPERATOR_KIND_##x,
#include "minsc/code_analysis/binding/bound_binary_operator_kind.inc"
#undef X
} BoundBinaryOperatorKind;

typedef struct BoundBinaryOperator {
    SyntaxKind syntax_kind;
    BoundBinaryOperatorKind kind;
    ObjectType left_type;
    ObjectType right_type;
    ObjectType result_type;
} BoundBinaryOperator;

const BoundBinaryOperator*
bind_binary_operator(SyntaxKind syntax_kind, ObjectType left_type, ObjectType right_type);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_BINARY_OPERATOR_H_
