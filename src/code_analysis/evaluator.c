#include "minsc/code_analysis/evaluator.h"

#include <stdio.h>
#include <stdlib.h>

#include "minsc/code_analysis/binding/bound_binary_expression.h"
#include "minsc/code_analysis/binding/bound_literal_expression.h"
#include "minsc/code_analysis/binding/bound_unary_expression.h"
#include "minsc/runtime/object.h"
#include "minsc/support/minsc_assert.h"

struct Evaluator {
    BoundExpression* root;
};

static Object* evaluate_expression(BoundExpression* expression);

Evaluator* evaluator_new(BoundExpression* root) {
    Evaluator* evaluator = malloc(sizeof(Evaluator));
    MINSC_ASSERT(evaluator != NULL);
    evaluator->root = root;
    return evaluator;
}

void evaluator_free(Evaluator* evaluator) {
    free(evaluator);
}

Object* evaluator_evaluate(Evaluator* evaluator) {
    return evaluate_expression(evaluator->root);
}

static Object* evaluate_expression(BoundExpression* expression) {
    switch (expression->type) {
        case BOUND_EXPRESSION_TYPE_LITERAL:
            return object_dup(((BoundLiteralExpression*)expression)->value);
        case BOUND_EXPRESSION_TYPE_BINARY: {
            BoundBinaryExpression* binary = (BoundBinaryExpression*)expression;
            Object* left = evaluate_expression(binary->left);
            Object* right = evaluate_expression(binary->right);
            switch (binary->op->kind) {
                case BOUND_BINARY_OPERATOR_KIND_ADDITION:
                    return object_new_i64(
                        object_unwrap_i64(left) + object_unwrap_i64(right)
                    );
                case BOUND_BINARY_OPERATOR_KIND_SUBTRACTION:
                    return object_new_i64(
                        object_unwrap_i64(left) - object_unwrap_i64(right)
                    );
                case BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION:
                    return object_new_i64(
                        object_unwrap_i64(left) * object_unwrap_i64(right)
                    );
                case BOUND_BINARY_OPERATOR_KIND_DIVISION:
                    return object_new_i64(
                        object_unwrap_i64(left) / object_unwrap_i64(right)
                    );
                case BOUND_BINARY_OPERATOR_KIND_LOGICAL_AND: {
                    bool left_b = object_unwrap_bool(left);
                    bool right_b = object_unwrap_bool(right);
                    return object_new_bool(left_b && right_b);
                }
                case BOUND_BINARY_OPERATOR_KIND_LOGICAL_OR: {
                    bool left_b = object_unwrap_bool(left);
                    bool right_b = object_unwrap_bool(right);
                    return object_new_bool(left_b || right_b);
                }
                default:
                    MINSC_ABORT("Unexpected binary operator");
            }
        }
        case BOUND_EXPRESSION_TYPE_UNARY: {
            BoundUnaryExpression* unary = (BoundUnaryExpression*)expression;
            Object* operand = evaluate_expression(unary->operand);
            switch (unary->op->kind) {
                case BOUND_UNARY_OPERATOR_KIND_IDENTITY:
                    return operand;
                case BOUND_UNARY_OPERATOR_KIND_NEGATION:
                    return object_new_i64(-object_unwrap_i64(operand));
                case BOUND_UNARY_OPERATOR_KIND_LOGICAL_NEGATION:
                    return object_new_bool(!object_unwrap_bool(operand));
                default:
                    MINSC_ABORT("Unexpected unary operator");
            }
        }
    }

    MINSC_ABORT("corrupt expression or not handled in switch");
}
