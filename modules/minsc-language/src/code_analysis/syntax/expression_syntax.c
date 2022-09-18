#include "minsc/code_analysis/syntax/expression_syntax.h"

#include "minsc/code_analysis/syntax/assignment_expression_syntax.h"
#include "minsc/code_analysis/syntax/binary_expression_syntax.h"
#include "minsc/code_analysis/syntax/literal_expression_syntax.h"
#include "minsc/code_analysis/syntax/name_expression_syntax.h"
#include "minsc/code_analysis/syntax/parenthesized_expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/unary_expression_syntax.h"
#include "minsc/support/minsc_assert.h"

SyntaxKind expression_syntax_kind(const ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            return SYNTAX_KIND_LITERAL_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_BINARY:
            return SYNTAX_KIND_BINARY_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_PARENTHESIZED:
            return SYNTAX_KIND_PARENTHESIZED_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_UNARY:
            return SYNTAX_KIND_UNARY_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_NAME:
            return SYNTAX_KIND_NAME_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_ASSIGNMENT:
            return SYNTAX_KIND_ASSIGNMENT_EXPRESSION;
    }

    MINSC_ABORT("Invalid expression syntax type");
}

void expression_syntax_free(ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            literal_expression_syntax_free((LiteralExpressionSyntax*)expression);
            break;
        case EXPRESSION_SYNTAX_TYPE_BINARY:
            binary_expression_syntax_free((BinaryExpressionSyntax*)expression);
            break;
        case EXPRESSION_SYNTAX_TYPE_PARENTHESIZED:
            parenthesized_expresion_syntax_free((ParenthesizedExpressionSyntax*)expression);
            break;
        case EXPRESSION_SYNTAX_TYPE_UNARY:
            unary_expression_syntax_free((UnaryExpressionSyntax*)expression);
            break;
        case EXPRESSION_SYNTAX_TYPE_NAME:
            name_expression_syntax_free((NameExpressionSyntax*)expression);
            break;
        case EXPRESSION_SYNTAX_TYPE_ASSIGNMENT:
            assignment_expression_syntax_free((AssignmentExpressionSyntax*)expression);
            break;
        default:
            MINSC_ABORT("Invalid expression syntax type");
    }
}

SyntaxNodeChildren expression_syntax_children(const ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            return literal_expression_syntax_children((const LiteralExpressionSyntax*)expression);
        case EXPRESSION_SYNTAX_TYPE_BINARY:
            return binary_expression_syntax_children((const BinaryExpressionSyntax*)expression);
        case EXPRESSION_SYNTAX_TYPE_PARENTHESIZED:
            return parenthesized_expression_syntax_children(
                (const ParenthesizedExpressionSyntax*)expression
            );
        case EXPRESSION_SYNTAX_TYPE_UNARY:
            return unary_expression_syntax_children((const UnaryExpressionSyntax*)expression);
        case EXPRESSION_SYNTAX_TYPE_NAME:
            return name_expression_syntax_children((const NameExpressionSyntax*)expression);
        case EXPRESSION_SYNTAX_TYPE_ASSIGNMENT:
            return assignment_expression_syntax_children(
                (const AssignmentExpressionSyntax*)expression
            );
    }

    MINSC_ABORT("Invalid expression syntax type");
}
