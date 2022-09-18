#include "minsc/code_analysis/binding/bound_expression.h"

#include "minsc/code_analysis/binding/bound_assignment_expression.h"
#include "minsc/code_analysis/binding/bound_binary_expression.h"
#include "minsc/code_analysis/binding/bound_literal_expression.h"
#include "minsc/code_analysis/binding/bound_node.h"
#include "minsc/code_analysis/binding/bound_unary_expression.h"
#include "minsc/code_analysis/binding/bound_variable_expression.h"
#include "minsc/support/minsc_assert.h"

void bound_expression_free(BoundExpression* expression) {
    switch (expression->type) {
        case BOUND_EXPRESSION_TYPE_UNARY:
            bound_unary_expression_free((BoundUnaryExpression*)expression);
            break;
        case BOUND_EXPRESSION_TYPE_LITERAL:
            bound_literal_expression_free((BoundLiteralExpression*)expression);
            break;
        case BOUND_EXPRESSION_TYPE_BINARY:
            bound_binary_expression_free((BoundBinaryExpression*)expression);
            break;
        case BOUND_EXPRESSION_TYPE_VARIABLE:
            bound_variable_expression_free((BoundVariableExpression*)expression);
            break;
        case BOUND_EXPRESSION_TYPE_ASSIGNMENT:
            bound_assignment_expression_free((BoundAssignmentExpression*)expression);
            break;
        default:
            MINSC_ABORT("Invalid bound expression type");
    }
}

BoundNodeKind bound_expression_kind(const BoundExpression* expression) {
    switch (expression->type) {
        case BOUND_EXPRESSION_TYPE_UNARY:
            return BOUND_NODE_KIND_UNARY_EXPRESSION;
        case BOUND_EXPRESSION_TYPE_LITERAL:
            return BOUND_NODE_KIND_LITERAL_EXPRESSION;
        case BOUND_EXPRESSION_TYPE_BINARY:
            return BOUND_NODE_KIND_BINARY_EXPRESSION;
        case BOUND_EXPRESSION_TYPE_VARIABLE:
            return BOUND_NODE_KIND_VARIABLE_EXPRESSION;
        case BOUND_EXPRESSION_TYPE_ASSIGNMENT:
            return BOUND_NODE_KIND_ASSIGNMENT_EXPRESSION;
    }

    MINSC_ABORT("Invalid bound expression type");
}

ObjectType bound_expression_type(const BoundExpression* expression) {
    switch (expression->type) {
        case BOUND_EXPRESSION_TYPE_UNARY:
            return bound_unary_expression_type((const BoundUnaryExpression*)expression);
        case BOUND_EXPRESSION_TYPE_LITERAL:
            return bound_literal_expression_type((const BoundLiteralExpression*)expression);
        case BOUND_EXPRESSION_TYPE_BINARY:
            return bound_binary_expression_type((const BoundBinaryExpression*)expression);
        case BOUND_EXPRESSION_TYPE_VARIABLE:
            return bound_variable_expression_type((const BoundVariableExpression*)expression);
        case BOUND_EXPRESSION_TYPE_ASSIGNMENT:
            return bound_assignment_expression_type((const BoundAssignmentExpression*)expression);
    }

    MINSC_ABORT("Invalid bound expression type");
}
