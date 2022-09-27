#ifndef MINSC_CODE_ANALYSIS_SYNTAX_COMPILATION_UNIT_SYNTAX_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_COMPILATION_UNIT_SYNTAX_H_

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/code_analysis/syntax/syntax_token.h"

typedef struct CompilationUnit {
    SyntaxNode node;
    ExpressionSyntax* expression;
    SyntaxToken* end_of_file_token;
} CompilationUnit;

CompilationUnit* compilation_unit_new(ExpressionSyntax* expression, SyntaxToken* end_of_file_token);
void compilation_unit_free(CompilationUnit* unit);

SyntaxNodeChildren compilation_unit_children(const CompilationUnit* unit);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_COMPILATION_UNIT_SYNTAX_H_
