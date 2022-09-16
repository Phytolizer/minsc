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

static int64_t evaluate_expression(BoundExpression* expression);

Evaluator* evaluator_new(BoundExpression* root) {
    Evaluator* evaluator = malloc(sizeof(Evaluator));
    MINSC_ASSERT(evaluator != NULL);
    evaluator->root = root;
    return evaluator;
}

void evaluator_free(Evaluator* evaluator) {
    free(evaluator);
}

int64_t evaluator_evaluate(Evaluator* evaluator) {
    return evaluate_expression(evaluator->root);
}

static int64_t evaluate_expression(BoundExpression* expression) {
    switch (expression->type) {
        case BOUND_EXPRESSION_TYPE_LITERAL:
            return object_as_i64(((BoundLiteralExpression*)expression)->value);
        case BOUND_EXPRESSION_TYPE_BINARY: {
            BoundBinaryExpression* binary = (BoundBinaryExpression*)expression;
            int64_t left = evaluate_expression(binary->left);
            int64_t right = evaluate_expression(binary->right);
            switch (binary->operator_kind) {
                case BOUND_BINARY_OPERATOR_KIND_ADDITION:
                    return left + right;
                case BOUND_BINARY_OPERATOR_KIND_SUBTRACTION:
                    return left - right;
                case BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION:
                    return left * right;
                case BOUND_BINARY_OPERATOR_KIND_DIVISION:
                    return left / right;
                default:
                    MINSC_ABORT("Unexpected binary operator");
            }
        }
        case BOUND_EXPRESSION_TYPE_UNARY: {
            BoundUnaryExpression* unary = (BoundUnaryExpression*)expression;
            int64_t operand = evaluate_expression(unary->operand);
            switch (unary->operator_kind) {
                case BOUND_UNARY_OPERATOR_KIND_IDENTITY:
                    return operand;
                case BOUND_UNARY_OPERATOR_KIND_NEGATION:
                    return -operand;
                default:
                    MINSC_ABORT("Unexpected unary operator");
            }
        }
    }

    MINSC_ABORT("corrupt expression or not handled in switch");
}
