#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TREE_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TREE_H_

#include <str/str.h>

#include "minsc/code_analysis/syntax/diagnostic.h"
#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_token.h"

typedef struct SyntaxTree {
    DiagnosticBuf diagnostics;
    ExpressionSyntax* root;
    SyntaxToken* end_of_file_token;
} SyntaxTree;

SyntaxTree*
syntax_tree_new(DiagnosticBuf diagnostics, ExpressionSyntax* root, SyntaxToken* end_of_file_token);
void syntax_tree_free(SyntaxTree* tree);

DiagnosticBuf syntax_tree_take_diagnostics(SyntaxTree* tree);
SyntaxTree* syntax_tree_parse(str text);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TREE_H_
