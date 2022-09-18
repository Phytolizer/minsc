#ifndef MINSC_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_SYNTAX_H_

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/runtime/object.h"

typedef struct LiteralExpressionSyntax {
    ExpressionSyntax base;
    SyntaxToken* literal_token;
    Object* value;
} LiteralExpressionSyntax;

ExpressionSyntax* literal_expression_syntax_new(SyntaxToken* literal_token, Object* value);
void literal_expression_syntax_free(LiteralExpressionSyntax* expression);

SyntaxNodeChildren literal_expression_syntax_children(const LiteralExpressionSyntax* expression);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_SYNTAX_H_
