#include "minsc/code_analysis/syntax/parenthesized_expression_syntax.h"

#include <buf/buf.h>
#include <stdlib.h>

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/support/minsc_assert.h"

ExpressionSyntax* parenthesized_expression_syntax_new(
    SyntaxToken* open_parenthesis_token,
    ExpressionSyntax* expression,
    SyntaxToken* close_parenthesis_token
) {
    ParenthesizedExpressionSyntax* syntax = malloc(sizeof(ParenthesizedExpressionSyntax));
    MINSC_ASSERT(syntax != NULL);
    syntax->base.base.type = SYNTAX_NODE_TYPE_EXPRESSION;
    syntax->base.type = EXPRESSION_SYNTAX_TYPE_PARENTHESIZED;
    syntax->open_parenthesis_token = open_parenthesis_token;
    syntax->expression = expression;
    syntax->close_parenthesis_token = close_parenthesis_token;
    return (ExpressionSyntax*)syntax;
}

void parenthesized_expresion_syntax_free(ParenthesizedExpressionSyntax* syntax) {
    syntax_token_free(syntax->open_parenthesis_token);
    expression_syntax_free(syntax->expression);
    syntax_token_free(syntax->close_parenthesis_token);
    free(syntax);
}

SyntaxNodeChildren parenthesized_expression_syntax_children(
    const ParenthesizedExpressionSyntax* syntax
) {
    SyntaxNodeChildren children = BUF_NEW;
    BUF_PUSH(&children, (const SyntaxNode*)syntax->open_parenthesis_token);
    BUF_PUSH(&children, (const SyntaxNode*)syntax->expression);
    BUF_PUSH(&children, (const SyntaxNode*)syntax->close_parenthesis_token);
    return children;
}
