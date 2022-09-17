#include "minsc/code_analysis/binding/bound_unary_operator.h"

static const BoundUnaryOperator OPERATORS[] = {
    {
        .syntax_kind = SYNTAX_KIND_PLUS_TOKEN,
        .kind = BOUND_UNARY_OPERATOR_KIND_IDENTITY,
        .operand_type = OBJECT_TYPE_INT64,
        .result_type = OBJECT_TYPE_INT64,
    },
    {
        .syntax_kind = SYNTAX_KIND_MINUS_TOKEN,
        .kind = BOUND_UNARY_OPERATOR_KIND_NEGATION,
        .operand_type = OBJECT_TYPE_INT64,
        .result_type = OBJECT_TYPE_INT64,
    },
    {
        .syntax_kind = SYNTAX_KIND_BANG_TOKEN,
        .kind = BOUND_UNARY_OPERATOR_KIND_LOGICAL_NEGATION,
        .operand_type = OBJECT_TYPE_BOOL,
        .result_type = OBJECT_TYPE_BOOL,
    },
};

static const size_t NUM_OPERATORS = sizeof(OPERATORS) / sizeof(OPERATORS[0]);

const BoundUnaryOperator* bind_unary_operator(SyntaxKind syntax_kind, ObjectType operand_type) {
    for (size_t i = 0; i < NUM_OPERATORS; i++) {
        const BoundUnaryOperator* op = &OPERATORS[i];
        if (op->syntax_kind == syntax_kind && op->operand_type == operand_type) {
            return op;
        }
    }
    return NULL;
}
