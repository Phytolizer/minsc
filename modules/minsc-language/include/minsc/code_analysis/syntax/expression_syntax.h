#ifndef MINSC_CODE_ANALYSIS_SYNTAX_EXPRESSION_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_EXPRESSION_SYNTAX_H_

#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_node.h"

typedef enum {
#define X(x) EXPRESSION_SYNTAX_TYPE_##x,
#include "minsc/code_analysis/syntax/expression_syntax_type.inc"
#undef X
} ExpressionSyntaxType;

typedef struct ExpressionSyntax {
    SyntaxNode base;
    ExpressionSyntaxType type;
} ExpressionSyntax;

SyntaxKind expression_syntax_kind(const ExpressionSyntax* expression);
void expression_syntax_free(ExpressionSyntax* expression);

SyntaxNodeChildren expression_syntax_children(const ExpressionSyntax* expression);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_EXPRESSION_SYNTAX_H_
