#ifndef MINSC_CODE_ANALYSIS_SYNTAX_BINARY_EXPRESSION_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_BINARY_EXPRESSION_SYNTAX_H_

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"

typedef struct BinaryExpressionSyntax {
    ExpressionSyntax base;
    ExpressionSyntax* left;
    SyntaxToken* operator_token;
    ExpressionSyntax* right;
} BinaryExpressionSyntax;

ExpressionSyntax* binary_expression_syntax_new(
    ExpressionSyntax* left,
    SyntaxToken* operator_token,
    ExpressionSyntax* right
);
void binary_expression_syntax_free(BinaryExpressionSyntax* expression);

SyntaxNodeChildren binary_expression_syntax_children(const BinaryExpressionSyntax* expression);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_BINARY_EXPRESSION_SYNTAX_H_
