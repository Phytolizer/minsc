#include "minsc/code_analysis/syntax/assignment_expression_syntax.h"

#include "minsc/support/minsc_assert.h"

ExpressionSyntax* assignment_expression_syntax_new(
    SyntaxToken* identifier_token,
    SyntaxToken* equals_token,
    ExpressionSyntax* expression
) {
    AssignmentExpressionSyntax* assignment_expression = malloc(sizeof(AssignmentExpressionSyntax));
    MINSC_ASSERT(assignment_expression != NULL);
    assignment_expression->base.base.type = SYNTAX_NODE_TYPE_EXPRESSION;
    assignment_expression->base.type = EXPRESSION_SYNTAX_TYPE_ASSIGNMENT;
    assignment_expression->identifier_token = identifier_token;
    assignment_expression->equals_token = equals_token;
    assignment_expression->expression = expression;
    return (ExpressionSyntax*)assignment_expression;
}

void assignment_expression_syntax_free(AssignmentExpressionSyntax* expression) {
    syntax_token_free(expression->identifier_token);
    syntax_token_free(expression->equals_token);
    expression_syntax_free(expression->expression);
    free(expression);
}

SyntaxNodeChildren assignment_expression_syntax_children(
    const AssignmentExpressionSyntax* expression
) {
    SyntaxNodeChildren children = BUF_NEW;
    BUF_PUSH(&children, (SyntaxNode*)expression->identifier_token);
    BUF_PUSH(&children, (SyntaxNode*)expression->equals_token);
    BUF_PUSH(&children, (SyntaxNode*)expression->expression);
    return children;
}
