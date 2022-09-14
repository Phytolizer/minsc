#include "syntax_tree.h"

#include <stdlib.h>

#include "minsc_assert.h"

SyntaxTree* syntax_tree_new(DiagnosticBuf diagnostics,
                            ExpressionSyntax* root,
                            SyntaxToken* end_of_file_token) {
    SyntaxTree* tree = malloc(sizeof(SyntaxTree));
    MINSC_ASSERT(tree != NULL);
    tree->diagnostics = diagnostics;
    tree->root = root;
    tree->end_of_file_token = end_of_file_token;
    return tree;
}

void syntax_tree_free(SyntaxTree* tree) {
    diagnostic_buf_free(tree->diagnostics);
    expression_syntax_free(tree->root);
    syntax_token_free(tree->end_of_file_token);
    free(tree);
}
