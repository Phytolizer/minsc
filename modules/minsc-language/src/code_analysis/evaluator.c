#include "minsc/code_analysis/evaluator.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "minsc/code_analysis/binding/bound_assignment_expression.h"
#include "minsc/code_analysis/binding/bound_binary_expression.h"
#include "minsc/code_analysis/binding/bound_binary_operator.h"
#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/binding/bound_literal_expression.h"
#include "minsc/code_analysis/binding/bound_unary_expression.h"
#include "minsc/code_analysis/binding/bound_unary_operator.h"
#include "minsc/code_analysis/binding/bound_variable_expression.h"
#include "minsc/code_analysis/variable_symbol.h"
#include "minsc/runtime/object.h"
#include "minsc/support/minsc_assert.h"

struct Evaluator {
    BoundExpression* root;
    VariableMap** variables;
};

static Object* evaluate_expression(Evaluator* evaluator, BoundExpression* expression);
static Object* evaluate_literal_expression(Evaluator* evaluator, BoundLiteralExpression* literal);
static Object* evaluate_unary_expression(Evaluator* evaluator, BoundUnaryExpression* unary);
static Object* evaluate_binary_expression(Evaluator* evaluator, BoundBinaryExpression* binary);
static Object*
evaluate_variable_expression(Evaluator* evaluator, BoundVariableExpression* variable);
static Object*
evaluate_assignment_expression(Evaluator* evaluator, BoundAssignmentExpression* assignment);

Evaluator* evaluator_new(BoundExpression* root, VariableMap** variables) {
    Evaluator* evaluator = malloc(sizeof(Evaluator));
    MINSC_ASSERT(evaluator != NULL);
    evaluator->root = root;
    evaluator->variables = variables;
    return evaluator;
}

void evaluator_free(Evaluator* evaluator) {
    free(evaluator);
}

Object* evaluator_evaluate(Evaluator* evaluator) {
    return evaluate_expression(evaluator, evaluator->root);
}

static Object* evaluate_expression(Evaluator* evaluator, BoundExpression* expression) {
    Object* result;

    switch (expression->type) {
        case BOUND_EXPRESSION_TYPE_LITERAL: {
            BoundLiteralExpression* literal = (BoundLiteralExpression*)expression;
            result = evaluate_literal_expression(evaluator, literal);
        } break;
        case BOUND_EXPRESSION_TYPE_BINARY: {
            BoundBinaryExpression* binary = (BoundBinaryExpression*)expression;
            result = evaluate_binary_expression(evaluator, binary);
        } break;
        case BOUND_EXPRESSION_TYPE_UNARY: {
            BoundUnaryExpression* unary = (BoundUnaryExpression*)expression;
            result = evaluate_unary_expression(evaluator, unary);
        } break;
        case BOUND_EXPRESSION_TYPE_VARIABLE: {
            BoundVariableExpression* variable = (BoundVariableExpression*)expression;
            result = evaluate_variable_expression(evaluator, variable);
        } break;
        case BOUND_EXPRESSION_TYPE_ASSIGNMENT: {
            BoundAssignmentExpression* assignment = (BoundAssignmentExpression*)expression;
            result = evaluate_assignment_expression(evaluator, assignment);
        } break;
        default:
            MINSC_ABORT("corrupt expression or not handled in switch");
    }

    return result;
}

static Object* evaluate_literal_expression(Evaluator* evaluator, BoundLiteralExpression* literal) {
    (void)evaluator;
    return object_dup(literal->value);
}

static Object* evaluate_unary_expression(Evaluator* evaluator, BoundUnaryExpression* unary) {
    Object* result;

    Object* operand = evaluate_expression(evaluator, unary->operand);
    switch (unary->op->kind) {
        case BOUND_UNARY_OPERATOR_KIND_IDENTITY:
            result = operand;
            break;
        case BOUND_UNARY_OPERATOR_KIND_NEGATION:
            result = object_new_i64(-object_unwrap_i64(operand));
            break;
        case BOUND_UNARY_OPERATOR_KIND_LOGICAL_NEGATION:
            result = object_new_bool(!object_unwrap_bool(operand));
            break;
        default:
            MINSC_ABORT("Unexpected unary operator");
    }

    return result;
}

static Object* evaluate_binary_expression(Evaluator* evaluator, BoundBinaryExpression* binary) {
    Object* result;

    Object* left = evaluate_expression(evaluator, binary->left);
    Object* right = evaluate_expression(evaluator, binary->right);
    switch (binary->op->kind) {
        case BOUND_BINARY_OPERATOR_KIND_ADDITION:
            result = object_new_i64(object_unwrap_i64(left) + object_unwrap_i64(right));
            break;
        case BOUND_BINARY_OPERATOR_KIND_SUBTRACTION:
            result = object_new_i64(object_unwrap_i64(left) - object_unwrap_i64(right));
            break;
        case BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION:
            result = object_new_i64(object_unwrap_i64(left) * object_unwrap_i64(right));
            break;
        case BOUND_BINARY_OPERATOR_KIND_DIVISION:
            result = object_new_i64(object_unwrap_i64(left) / object_unwrap_i64(right));
            break;
        case BOUND_BINARY_OPERATOR_KIND_LOGICAL_AND: {
            bool left_b = object_unwrap_bool(left);
            bool right_b = object_unwrap_bool(right);
            result = object_new_bool(left_b && right_b);
        } break;
        case BOUND_BINARY_OPERATOR_KIND_LOGICAL_OR: {
            bool left_b = object_unwrap_bool(left);
            bool right_b = object_unwrap_bool(right);
            result = object_new_bool(left_b || right_b);
        } break;
        case BOUND_BINARY_OPERATOR_KIND_EQUALITY: {
            if (left->type == OBJECT_TYPE_BOOL) {
                bool left_b = object_unwrap_bool(left);
                bool right_b = object_unwrap_bool(right);
                result = object_new_bool(left_b == right_b);
            } else if (left->type == OBJECT_TYPE_INT64) {
                int64_t left_i64 = object_unwrap_i64(left);
                int64_t right_i64 = object_unwrap_i64(right);
                result = object_new_bool(left_i64 == right_i64);
            } else {
                MINSC_ABORT("Unexpected type");
            }
        } break;
        case BOUND_BINARY_OPERATOR_KIND_INEQUALITY: {
            if (left->type == OBJECT_TYPE_BOOL) {
                bool left_b = object_unwrap_bool(left);
                bool right_b = object_unwrap_bool(right);
                result = object_new_bool(left_b != right_b);
            } else if (left->type == OBJECT_TYPE_INT64) {
                int64_t left_i64 = object_unwrap_i64(left);
                int64_t right_i64 = object_unwrap_i64(right);
                result = object_new_bool(left_i64 != right_i64);
            } else {
                MINSC_ABORT("Unexpected type");
            }
        } break;
        default:
            MINSC_ABORT("Unexpected binary operator");
    }

    return result;
}

static Object*
evaluate_variable_expression(Evaluator* evaluator, BoundVariableExpression* variable) {
    return object_dup(variable_map_get(*evaluator->variables, variable->variable));
}

static Object*
evaluate_assignment_expression(Evaluator* evaluator, BoundAssignmentExpression* assignment) {
    Object* value = evaluate_expression(evaluator, assignment->expression);
    variable_map_define(
        evaluator->variables,
        variable_symbol_dup(assignment->variable),
        object_dup(value)
    );
    return value;
}
