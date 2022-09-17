#include "minsc/code_analysis/syntax/literal_expression_syntax.h"

#include <assert.h>
#include <buf/buf.h>
#include <stdlib.h>

#include "minsc/support/is_derived.h"
#include "minsc/support/minsc_assert.h"

ExpressionSyntax*
literal_expression_syntax_new(SyntaxToken* literal_token, Object* value) {
    LiteralExpressionSyntax* expression =
        malloc(sizeof(LiteralExpressionSyntax));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = SYNTAX_NODE_TYPE_EXPRESSION;
    expression->base.type = EXPRESSION_SYNTAX_TYPE_LITERAL;
    expression->literal_token = literal_token;
    if (value == NULL) {
        expression->value = object_dup(expression->literal_token->value);
    } else {
        expression->value = value;
    }
    return (ExpressionSyntax*)expression;
}

void literal_expression_syntax_free(LiteralExpressionSyntax* expression) {
    syntax_token_free(expression->literal_token);
    object_free(expression->value);
    free(expression);
}

SyntaxNodeChildren literal_expression_syntax_children(
    const LiteralExpressionSyntax* expression
) {
    SyntaxNodeChildren children = BUF_NEW;
    static_assert(
        IS_DERIVED(SyntaxNode, expression->literal_token),
        "LiteralExpressionSyntax literal_token must be a SyntaxNode"
    );
    BUF_PUSH(&children, (SyntaxNode*)expression->literal_token);
    return children;
}
