#include "minsc/code_analysis/syntax/syntax_tree.h"

#include <buf/buf.h>
#include <stdlib.h>
#include <str/str.h>

#include "minsc/code_analysis/syntax/diagnostic.h"
#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/parser.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/support/minsc_assert.h"

SyntaxTree*
syntax_tree_new(DiagnosticBuf diagnostics, ExpressionSyntax* root, SyntaxToken* end_of_file_token) {
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

DiagnosticBuf syntax_tree_take_diagnostics(SyntaxTree* tree) {
    DiagnosticBuf diagnostics = tree->diagnostics;
    tree->diagnostics = (DiagnosticBuf)BUF_NEW;
    return diagnostics;
}

SyntaxTree* syntax_tree_parse(str text) {
    Parser* parser = parser_new(text);
    SyntaxTree* tree = parser_parse(parser);
    parser_free(parser);
    return tree;
}
