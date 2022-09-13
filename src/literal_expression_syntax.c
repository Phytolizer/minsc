#include "literal_expression_syntax.h"

#include <assert.h>
#include <stdlib.h>

#include "is_derived.h"

ExpressionSyntax* literal_expression_syntax_new(SyntaxToken* literal_token) {
    LiteralExpressionSyntax* expression =
            malloc(sizeof(LiteralExpressionSyntax));
    expression->base.base.type = SYNTAX_NODE_TYPE_EXPRESSION;
    expression->base.type = EXPRESSION_SYNTAX_TYPE_LITERAL;
    expression->literal_token = literal_token;
    return (ExpressionSyntax*)expression;
}

void literal_expression_syntax_free(LiteralExpressionSyntax* expression) {
    syntax_token_free(expression->literal_token);
    free(expression);
}

SyntaxNodeChildren literal_expression_syntax_children(
        const LiteralExpressionSyntax* expression) {
    SyntaxNodeChildren children = BUF_NEW;
    static_assert(IS_DERIVED(SyntaxNode, expression->literal_token),
                  "LiteralExpressionSyntax literal_token must be a SyntaxNode");
    BUF_PUSH(&children, (SyntaxNode*)expression->literal_token);
    return children;
}
