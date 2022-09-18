#ifndef MINSC_CODE_ANALYSIS_SYNTAX_ASSIGNMENT_EXPRESSION_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_ASSIGNMENT_EXPRESSION_SYNTAX_H_

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"

typedef struct AssignmentExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken* identifier_token;
    SyntaxToken* equals_token;
    ExpressionSyntax* expression;
} AssignmentExpressionSyntax;

ExpressionSyntax* assignment_expression_syntax_new(
    SyntaxToken* identifier_token,
    SyntaxToken* equals_token,
    ExpressionSyntax* expression
);
void assignment_expression_syntax_free(AssignmentExpressionSyntax* expression);

SyntaxNodeChildren assignment_expression_syntax_children(
    const AssignmentExpressionSyntax* expression
);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_ASSIGNMENT_EXPRESSION_SYNTAX_H_
