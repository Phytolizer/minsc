#include "minsc/code_analysis/binding/binder.h"

#include <buf/buf.h>
#include <stdlib.h>
#include <str/str.h>

#include "minsc/code_analysis/binding/bound_assignment_expression.h"
#include "minsc/code_analysis/binding/bound_binary_expression.h"
#include "minsc/code_analysis/binding/bound_binary_operator.h"
#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/binding/bound_literal_expression.h"
#include "minsc/code_analysis/binding/bound_unary_expression.h"
#include "minsc/code_analysis/binding/bound_unary_operator.h"
#include "minsc/code_analysis/binding/bound_variable_expression.h"
#include "minsc/code_analysis/syntax/assignment_expression_syntax.h"
#include "minsc/code_analysis/syntax/binary_expression_syntax.h"
#include "minsc/code_analysis/syntax/literal_expression_syntax.h"
#include "minsc/code_analysis/syntax/name_expression_syntax.h"
#include "minsc/code_analysis/syntax/parenthesized_expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/code_analysis/syntax/unary_expression_syntax.h"
#include "minsc/runtime/object.h"
#include "minsc/support/minsc_assert.h"

struct Binder {
    DiagnosticBag* diagnostics;
    VariableMap* variables;
};

static BoundExpression* bind_literal_expression(Binder* binder, LiteralExpressionSyntax* syntax);
static BoundExpression* bind_unary_expression(Binder* binder, UnaryExpressionSyntax* syntax);
static BoundExpression* bind_binary_expression(Binder* binder, BinaryExpressionSyntax* syntax);
static BoundExpression*
bind_parenthesized_expression(Binder* binder, ParenthesizedExpressionSyntax* syntax);
static BoundExpression* bind_name_expression(Binder* binder, NameExpressionSyntax* syntax);
static BoundExpression*
bind_assignment_expression(Binder* binder, AssignmentExpressionSyntax* syntax);

Binder* binder_new(VariableMap* variables) {
    Binder* binder = malloc(sizeof(Binder));
    MINSC_ASSERT(binder != NULL);
    binder->diagnostics = diagnostic_bag_new();
    binder->variables = variables;
    return binder;
}

void binder_free(Binder* binder) {
    diagnostic_bag_free(binder->diagnostics);
    // don't free variables, they could be reused
    free(binder);
}

BoundExpression* binder_bind_expression(Binder* binder, ExpressionSyntax* syntax) {
    switch (expression_syntax_kind(syntax)) {
        case SYNTAX_KIND_LITERAL_EXPRESSION:
            return bind_literal_expression(binder, (LiteralExpressionSyntax*)syntax);
        case SYNTAX_KIND_UNARY_EXPRESSION:
            return bind_unary_expression(binder, (UnaryExpressionSyntax*)syntax);
        case SYNTAX_KIND_BINARY_EXPRESSION:
            return bind_binary_expression(binder, (BinaryExpressionSyntax*)syntax);
        case SYNTAX_KIND_PARENTHESIZED_EXPRESSION:
            return bind_parenthesized_expression(binder, (ParenthesizedExpressionSyntax*)syntax);
        case SYNTAX_KIND_NAME_EXPRESSION:
            return bind_name_expression(binder, (NameExpressionSyntax*)syntax);
        case SYNTAX_KIND_ASSIGNMENT_EXPRESSION:
            return bind_assignment_expression(binder, (AssignmentExpressionSyntax*)syntax);
        default:
            MINSC_ABORT("Unexpected expression syntax kind");
    }
}

DiagnosticBag* binder_take_diagnostics(Binder* binder) {
    DiagnosticBag* diagnostics = binder->diagnostics;
    binder->diagnostics = NULL;
    return diagnostics;
}

static BoundExpression* bind_literal_expression(Binder* binder, LiteralExpressionSyntax* syntax) {
    (void)binder;
    return bound_literal_expression_new(object_dup(syntax->value));
}

static BoundExpression* bind_unary_expression(Binder* binder, UnaryExpressionSyntax* syntax) {
    BoundExpression* operand = binder_bind_expression(binder, syntax->operand);
    const BoundUnaryOperator* op =
        bind_unary_operator(syntax->operator_token->kind, bound_expression_type(operand));
    if (op == NULL) {
        diagnostic_bag_report_undefined_unary_operator(
            binder->diagnostics,
            syntax_token_span(syntax->operator_token),
            syntax->operator_token->text,
            bound_expression_type(operand)
        );
        return operand;
    }
    return bound_unary_expression_new(op, operand);
}

static BoundExpression* bind_binary_expression(Binder* binder, BinaryExpressionSyntax* syntax) {
    (void)binder;
    BoundExpression* left = binder_bind_expression(binder, syntax->left);
    BoundExpression* right = binder_bind_expression(binder, syntax->right);
    const BoundBinaryOperator* op = bind_binary_operator(
        syntax->operator_token->kind,
        bound_expression_type(left),
        bound_expression_type(right)
    );
    if (op == NULL) {
        diagnostic_bag_report_undefined_binary_operator(
            binder->diagnostics,
            syntax_token_span(syntax->operator_token),
            syntax->operator_token->text,
            bound_expression_type(left),
            bound_expression_type(right)
        );
        bound_expression_free(right);
        return left;
    }
    return bound_binary_expression_new(op, left, right);
}

static BoundExpression*
bind_parenthesized_expression(Binder* binder, ParenthesizedExpressionSyntax* syntax) {
    return binder_bind_expression(binder, syntax->expression);
}

static BoundExpression* bind_name_expression(Binder* binder, NameExpressionSyntax* syntax) {
    str name = str_dup(syntax->identifier_token->text);
    Object* value = variable_map_get(binder->variables, name);
    if (value == NULL) {
        diagnostic_bag_report_undefined_name(
            binder->diagnostics,
            syntax_token_span(syntax->identifier_token),
            name
        );
        str_free(name);
        return bound_literal_expression_new(object_new_i64(0));
    }

    ObjectType type = value ? value->type : OBJECT_TYPE_NULL;
    return bound_variable_expression_new(name, type);
}

static BoundExpression*
bind_assignment_expression(Binder* binder, AssignmentExpressionSyntax* syntax) {
    str name = str_dup(syntax->identifier_token->text);
    BoundExpression* bound_expression = binder_bind_expression(binder, syntax->expression);
    return bound_assignment_expression_new(name, bound_expression);
}
