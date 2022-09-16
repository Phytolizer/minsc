#include "minsc/code_analysis/syntax/unary_expression_syntax.h"

#include "minsc/support/minsc_assert.h"

ExpressionSyntax* unary_expression_syntax_new(SyntaxToken* operator_token,
                                              ExpressionSyntax* operand) {
    UnaryExpressionSyntax* syntax = malloc(sizeof(UnaryExpressionSyntax));
    MINSC_ASSERT(syntax != NULL);
    syntax->base.base.type = SYNTAX_NODE_TYPE_EXPRESSION;
    syntax->base.type = EXPRESSION_SYNTAX_TYPE_UNARY;
    syntax->operator_token = operator_token;
    syntax->operand = operand;
    return (ExpressionSyntax*)syntax;
}

void unary_expression_syntax_free(UnaryExpressionSyntax* expression) {
    syntax_token_free(expression->operator_token);
    expression_syntax_free(expression->operand);
    free(expression);
}

SyntaxNodeChildren unary_expression_syntax_children(
        const UnaryExpressionSyntax* expression) {
    SyntaxNodeChildren children = BUF_NEW;
    BUF_PUSH(&children, (const SyntaxNode*)expression->operator_token);
    BUF_PUSH(&children, (const SyntaxNode*)expression->operand);
    return children;
}
