#include "minsc/code_analysis/binding/bound_binary_operator.h"

static const BoundBinaryOperator OPERATORS[] = {
    {
        .syntax_kind = SYNTAX_KIND_PLUS_TOKEN,
        .kind = BOUND_BINARY_OPERATOR_KIND_ADDITION,
        .left_type = OBJECT_TYPE_INT64,
        .right_type = OBJECT_TYPE_INT64,
        .result_type = OBJECT_TYPE_INT64,
    },
    {
        .syntax_kind = SYNTAX_KIND_MINUS_TOKEN,
        .kind = BOUND_BINARY_OPERATOR_KIND_SUBTRACTION,
        .left_type = OBJECT_TYPE_INT64,
        .right_type = OBJECT_TYPE_INT64,
        .result_type = OBJECT_TYPE_INT64,
    },
    {
        .syntax_kind = SYNTAX_KIND_STAR_TOKEN,
        .kind = BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION,
        .left_type = OBJECT_TYPE_INT64,
        .right_type = OBJECT_TYPE_INT64,
        .result_type = OBJECT_TYPE_INT64,
    },
    {
        .syntax_kind = SYNTAX_KIND_SLASH_TOKEN,
        .kind = BOUND_BINARY_OPERATOR_KIND_DIVISION,
        .left_type = OBJECT_TYPE_INT64,
        .right_type = OBJECT_TYPE_INT64,
        .result_type = OBJECT_TYPE_INT64,
    },
    {
        .syntax_kind = SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN,
        .kind = BOUND_BINARY_OPERATOR_KIND_LOGICAL_AND,
        .left_type = OBJECT_TYPE_BOOL,
        .right_type = OBJECT_TYPE_BOOL,
        .result_type = OBJECT_TYPE_BOOL,
    },
    {
        .syntax_kind = SYNTAX_KIND_PIPE_PIPE_TOKEN,
        .kind = BOUND_BINARY_OPERATOR_KIND_LOGICAL_OR,
        .left_type = OBJECT_TYPE_BOOL,
        .right_type = OBJECT_TYPE_BOOL,
        .result_type = OBJECT_TYPE_BOOL,
    },
};

static const size_t NUM_OPERATORS = sizeof(OPERATORS) / sizeof(OPERATORS[0]);

const BoundBinaryOperator* bind_binary_operator(
    SyntaxKind syntax_kind,
    ObjectType left_type,
    ObjectType right_type
) {
    for (size_t i = 0; i < NUM_OPERATORS; i++) {
        const BoundBinaryOperator* op = &OPERATORS[i];
        if (op->syntax_kind == syntax_kind && op->left_type == left_type &&
            op->right_type == right_type) {
            return op;
        }
    }

    return NULL;
}
