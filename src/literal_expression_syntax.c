#include "literal_expression_syntax.h"

#include <stdlib.h>

ExpressionSyntax* literal_expression_syntax_new(SyntaxToken literal_token) {
    LiteralExpressionSyntax* expression =
            malloc(sizeof(LiteralExpressionSyntax));
    expression->base.type = EXPRESSION_SYNTAX_TYPE_LITERAL;
    expression->literal_token = literal_token;
    return (ExpressionSyntax*)expression;
}

void literal_expression_syntax_free(LiteralExpressionSyntax* expression) {
    syntax_token_free(expression->literal_token);
    free(expression);
}
