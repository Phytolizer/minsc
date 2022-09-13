#include "expression_syntax.h"

#include <stdlib.h>

#include "binary_expression_syntax.h"
#include "literal_expression_syntax.h"

SyntaxKind expression_syntax_kind(const ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            return SYNTAX_KIND_LITERAL_EXPRESSION;
        case EXPRESSION_SYNTAX_TYPE_BINARY:
            return SYNTAX_KIND_BINARY_EXPRESSION;
    }

    abort();
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
    }

    abort();
}
