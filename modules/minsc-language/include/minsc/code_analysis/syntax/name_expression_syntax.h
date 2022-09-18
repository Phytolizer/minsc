#ifndef MINSC_CODE_ANALYSIS_SYNTAX_NAME_EXPRESSION_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_NAME_EXPRESSION_SYNTAX_H_

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"

typedef struct NameExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken* identifier_token;
} NameExpressionSyntax;

ExpressionSyntax* name_expression_syntax_new(SyntaxToken* identifier_token);
void name_expression_syntax_free(NameExpressionSyntax* expression);

SyntaxNodeChildren name_expression_syntax_children(const NameExpressionSyntax* expression);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_NAME_EXPRESSION_SYNTAX_H_
