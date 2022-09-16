#ifndef MINSC_SYNTAX_TREE_H_
#define MINSC_SYNTAX_TREE_H_

#include "diagnostic.h"
#include "expression_syntax.h"
#include "syntax_token.h"

typedef struct SyntaxTree {
    DiagnosticBuf diagnostics;
    ExpressionSyntax* root;
    SyntaxToken* end_of_file_token;
} SyntaxTree;

SyntaxTree* syntax_tree_new(DiagnosticBuf diagnostics,
                            ExpressionSyntax* root,
                            SyntaxToken* end_of_file_token);
void syntax_tree_free(SyntaxTree* tree);

SyntaxTree* syntax_tree_parse(str text);

#endif  // MINSC_SYNTAX_TREE_H_
