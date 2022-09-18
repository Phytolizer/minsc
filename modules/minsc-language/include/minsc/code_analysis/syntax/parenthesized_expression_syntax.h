#ifndef MINSC_CODE_ANALYSIS_SYNTAX_PARENTHESIZED_EXPRESSION_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_PARENTHESIZED_EXPRESSION_SYNTAX_H_

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"

typedef struct ParenthesizedExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken* open_parenthesis_token;
    ExpressionSyntax* expression;
    SyntaxToken* close_parenthesis_token;
} ParenthesizedExpressionSyntax;

ExpressionSyntax* parenthesized_expression_syntax_new(
    SyntaxToken* open_parenthesis_token,
    ExpressionSyntax* expression,
    SyntaxToken* close_parenthesis_token
);
void parenthesized_expresion_syntax_free(ParenthesizedExpressionSyntax* syntax);

SyntaxNodeChildren parenthesized_expression_syntax_children(
    const ParenthesizedExpressionSyntax* syntax
);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_PARENTHESIZED_EXPRESSION_SYNTAX_H_
