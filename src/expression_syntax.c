#include "minsc/code_analysis/expression_syntax.h"

#include <stdlib.h>

#include "minsc/code_analysis/binary_expression_syntax.h"
#include "minsc/code_analysis/literal_expression_syntax.h"
#include "minsc/code_analysis/parenthesized_expression_syntax.h"
#include "minsc/support/minsc_assert.h"

SyntaxKind expression_syntax_kind(const ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            return SYNTAX_KIND_LITERAL_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_BINARY:
            return SYNTAX_KIND_BINARY_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_PARENTHESIZED:
            return SYNTAX_KIND_PARENTHESIZED_EXPRESSION;
    }

    MINSC_ABORT("Invalid expression syntax type");
}

void expression_syntax_free(ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            literal_expression_syntax_free(
                    (LiteralExpressionSyntax*)expression);
            break;
        case EXPRESSION_SYNTAX_TYPE_BINARY:
            binary_expression_syntax_free((BinaryExpressionSyntax*)expression);
            break;
        case EXPRESSION_SYNTAX_TYPE_PARENTHESIZED:
            parenthesized_expresion_syntax_free(
                    (ParenthesizedExpressionSyntax*)expression);
            break;
        default:
            MINSC_ABORT("Invalid expression syntax type");
    }
}

SyntaxNodeChildren expression_syntax_children(
        const ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            return literal_expression_syntax_children(
                    (const LiteralExpressionSyntax*)expression);
        case EXPRESSION_SYNTAX_TYPE_BINARY:
            return binary_expression_syntax_children(
                    (const BinaryExpressionSyntax*)expression);
        case EXPRESSION_SYNTAX_TYPE_PARENTHESIZED:
            return parenthesized_expression_syntax_children(
                    (const ParenthesizedExpressionSyntax*)expression);
    }

    MINSC_ABORT("Invalid expression syntax type");
}
