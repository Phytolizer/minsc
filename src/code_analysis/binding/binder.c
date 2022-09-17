#include "minsc/code_analysis/binding/binder.h"

#include <sum/sum.h>

#include "minsc/code_analysis/binding/bound_binary_expression.h"
#include "minsc/code_analysis/binding/bound_literal_expression.h"
#include "minsc/code_analysis/binding/bound_unary_expression.h"
#include "minsc/code_analysis/syntax/binary_expression_syntax.h"
#include "minsc/code_analysis/syntax/diagnostic.h"
#include "minsc/code_analysis/syntax/literal_expression_syntax.h"
#include "minsc/code_analysis/syntax/parenthesized_expression_syntax.h"
#include "minsc/code_analysis/syntax/unary_expression_syntax.h"
#include "minsc/support/minsc_assert.h"

struct Binder {
    DiagnosticBuf diagnostics;
};

typedef SUM_MAYBE_TYPE(BoundUnaryOperatorKind) BindUnaryOperatorKindResult;
static BindUnaryOperatorKindResult
bind_unary_operator_kind(SyntaxKind kind, ObjectType operand_type);
typedef SUM_MAYBE_TYPE(BoundBinaryOperatorKind) BindBinaryOperatorKindResult;
static BindBinaryOperatorKindResult bind_binary_operator_kind(
    SyntaxKind kind,
    ObjectType left_type,
    ObjectType right_type
);

static BoundExpression*
bind_literal_expression(Binder* binder, LiteralExpressionSyntax* syntax);
static BoundExpression*
bind_unary_expression(Binder* binder, UnaryExpressionSyntax* syntax);
static BoundExpression*
bind_binary_expression(Binder* binder, BinaryExpressionSyntax* syntax);
static BoundExpression* bind_parenthesized_expression(
    Binder* binder,
    ParenthesizedExpressionSyntax* syntax
);

Binder* binder_new(void) {
    Binder* binder = malloc(sizeof(Binder));
    MINSC_ASSERT(binder != NULL);
    binder->diagnostics = (DiagnosticBuf)BUF_NEW;
    return binder;
}

void binder_free(Binder* binder) {
    diagnostic_buf_free(binder->diagnostics);
    free(binder);
}

BoundExpression*
binder_bind_expression(Binder* binder, ExpressionSyntax* syntax) {
    switch (expression_syntax_kind(syntax)) {
        case SYNTAX_KIND_LITERAL_EXPRESSION:
            return bind_literal_expression(
                binder,
                (LiteralExpressionSyntax*)syntax
            );
        case SYNTAX_KIND_UNARY_EXPRESSION:
            return bind_unary_expression(
                binder,
                (UnaryExpressionSyntax*)syntax
            );
        case SYNTAX_KIND_BINARY_EXPRESSION:
            return bind_binary_expression(
                binder,
                (BinaryExpressionSyntax*)syntax
            );
        case SYNTAX_KIND_PARENTHESIZED_EXPRESSION:
            return bind_parenthesized_expression(
                binder,
                (ParenthesizedExpressionSyntax*)syntax
            );
        default:
            MINSC_ABORT("Unexpected expression syntax kind");
    }
}

DiagnosticBuf binder_take_diagnostics(Binder* binder) {
    DiagnosticBuf diagnostics = binder->diagnostics;
    binder->diagnostics = (DiagnosticBuf)BUF_NEW;
    return diagnostics;
}

static BindUnaryOperatorKindResult
bind_unary_operator_kind(SyntaxKind kind, ObjectType operand_type) {
    if (operand_type != OBJECT_TYPE_INT64) {
        return (BindUnaryOperatorKindResult)SUM_NOTHING;
    }
    switch (kind) {
        case SYNTAX_KIND_PLUS_TOKEN:
            return (BindUnaryOperatorKindResult
            )SUM_JUST(BOUND_UNARY_OPERATOR_KIND_IDENTITY);
        case SYNTAX_KIND_MINUS_TOKEN:
            return (BindUnaryOperatorKindResult
            )SUM_JUST(BOUND_UNARY_OPERATOR_KIND_NEGATION);
        default:
            MINSC_ABORT("Unexpected unary operator kind");
    }
}

static BindBinaryOperatorKindResult bind_binary_operator_kind(
    SyntaxKind kind,
    ObjectType left_type,
    ObjectType right_type
) {
    if (left_type != OBJECT_TYPE_INT64 || right_type != OBJECT_TYPE_INT64) {
        return (BindBinaryOperatorKindResult)SUM_NOTHING;
    }
    switch (kind) {
        case SYNTAX_KIND_PLUS_TOKEN:
            return (BindBinaryOperatorKindResult
            )SUM_JUST(BOUND_BINARY_OPERATOR_KIND_ADDITION);
        case SYNTAX_KIND_MINUS_TOKEN:
            return (BindBinaryOperatorKindResult
            )SUM_JUST(BOUND_BINARY_OPERATOR_KIND_SUBTRACTION);
        case SYNTAX_KIND_STAR_TOKEN:
            return (BindBinaryOperatorKindResult
            )SUM_JUST(BOUND_BINARY_OPERATOR_KIND_MULTIPLICATION);
        case SYNTAX_KIND_SLASH_TOKEN:
            return (BindBinaryOperatorKindResult
            )SUM_JUST(BOUND_BINARY_OPERATOR_KIND_DIVISION);
        default:
            MINSC_ABORT("Unexpected binary operator kind");
    }
}

static BoundExpression*
bind_literal_expression(Binder* binder, LiteralExpressionSyntax* syntax) {
    (void)binder;
    return bound_literal_expression_new(object_dup(syntax->value));
}

static BoundExpression*
bind_unary_expression(Binder* binder, UnaryExpressionSyntax* syntax) {
    BoundExpression* operand = binder_bind_expression(binder, syntax->operand);
    BindUnaryOperatorKindResult operator_kind = bind_unary_operator_kind(
        syntax->operator_token->kind,
        bound_expression_type(operand)
    );
    if (!operator_kind.present) {
        BUF_PUSH(
            &binder->diagnostics,
            str_printf(
                "Unary operator '" str_fmt "' is not "
                "defined for type '" str_fmt "'",
                str_arg(syntax->operator_token->text),
                str_arg(object_type_string(bound_expression_type(operand)))
            )
        );
        return operand;
    }
    return bound_unary_expression_new(operator_kind.value, operand);
}

static BoundExpression*
bind_binary_expression(Binder* binder, BinaryExpressionSyntax* syntax) {
    (void)binder;
    BoundExpression* left = binder_bind_expression(binder, syntax->left);
    BoundExpression* right = binder_bind_expression(binder, syntax->right);
    BindBinaryOperatorKindResult operator_kind = bind_binary_operator_kind(
        syntax->operator_token->kind,
        bound_expression_type(left),
        bound_expression_type(right)
    );
    if (!operator_kind.present) {
        BUF_PUSH(
            &binder->diagnostics,
            str_printf(
                "Binary operator '" str_fmt "' is not "
                "defined for types '" str_fmt "' and '" str_fmt "'",
                str_arg(syntax->operator_token->text),
                str_arg(object_type_string(bound_expression_type(left))),
                str_arg(object_type_string(bound_expression_type(right)))
            )
        );
        bound_expression_free(right);
        return left;
    }
    return bound_binary_expression_new(operator_kind.value, left, right);
}

static BoundExpression* bind_parenthesized_expression(
    Binder* binder,
    ParenthesizedExpressionSyntax* syntax
) {
    return binder_bind_expression(binder, syntax->expression);
}
