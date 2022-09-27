#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TREE_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TREE_H_

#include <buf/buf.h>
#include <str/str.h>

#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/code_analysis/syntax/compilation_unit_syntax.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/code_analysis/text/source_text.h"

typedef struct SyntaxTree {
    SourceText source;
    DiagnosticBag* diagnostics;
    CompilationUnit* root;
} SyntaxTree;

SyntaxTree* syntax_tree_new(SourceText source, DiagnosticBag* diagnostics, CompilationUnit* root);
void syntax_tree_free(SyntaxTree* tree);

DiagnosticBag* syntax_tree_take_diagnostics(SyntaxTree* tree);
SyntaxTree* syntax_tree_parse(str text);

typedef BUF(SyntaxToken*) SyntaxTokenBuf;

SyntaxTokenBuf syntax_tree_parse_tokens(str text);
void syntax_token_buf_free(SyntaxTokenBuf buf);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TREE_H_
