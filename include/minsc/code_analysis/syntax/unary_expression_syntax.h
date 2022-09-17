#ifndef MINSC_CODE_ANALYSIS_SYNTAX_UNARY_EXPRESSION_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_UNARY_EXPRESSION_SYNTAX_H_

#include "expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "syntax_token.h"

typedef struct UnaryExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken* operator_token;
    ExpressionSyntax* operand;
} UnaryExpressionSyntax;

ExpressionSyntax*
unary_expression_syntax_new(SyntaxToken* operator_token, ExpressionSyntax* operand);
void unary_expression_syntax_free(UnaryExpressionSyntax* expression);

SyntaxNodeChildren unary_expression_syntax_children(const UnaryExpressionSyntax* expression);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_UNARY_EXPRESSION_SYNTAX_H_
