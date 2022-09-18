#include "minsc/code_analysis/syntax/name_expression_syntax.h"

#include "minsc/support/minsc_assert.h"

ExpressionSyntax* name_expression_syntax_new(SyntaxToken* identifier_token) {
    NameExpressionSyntax* expression = malloc(sizeof(NameExpressionSyntax));
    MINSC_ASSERT(expression != NULL);
    expression->base.base.type = SYNTAX_NODE_TYPE_EXPRESSION;
    expression->base.type = EXPRESSION_SYNTAX_TYPE_NAME;
    expression->identifier_token = identifier_token;
    return (ExpressionSyntax*)expression;
}

void name_expression_syntax_free(NameExpressionSyntax* expression) {
    syntax_token_free(expression->identifier_token);
    free(expression);
}

SyntaxNodeChildren name_expression_syntax_children(const NameExpressionSyntax* expression) {
    SyntaxNodeChildren children = BUF_NEW;
    BUF_PUSH(&children, (SyntaxNode*)expression->identifier_token);
    return children;
}
