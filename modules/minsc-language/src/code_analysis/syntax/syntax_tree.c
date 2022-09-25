#include "minsc/code_analysis/syntax/syntax_tree.h"

#include <buf/buf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <str/str.h>

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/lexer.h"
#include "minsc/code_analysis/syntax/parser.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/code_analysis/text/source_text.h"
#include "minsc/support/minsc_assert.h"

static SyntaxTree* syntax_tree_parse_source(SourceText source);
static SyntaxTokenBuf syntax_tree_parse_tokens_source(SourceText source);

SyntaxTree* syntax_tree_new(
    SourceText source,
    DiagnosticBag* diagnostics,
    ExpressionSyntax* root,
    SyntaxToken* end_of_file_token
) {
    SyntaxTree* tree = malloc(sizeof(SyntaxTree));
    MINSC_ASSERT(tree != NULL);
    tree->source = source;
    tree->diagnostics = diagnostics;
    tree->root = root;
    tree->end_of_file_token = end_of_file_token;
    return tree;
}

void syntax_tree_free(SyntaxTree* tree) {
    diagnostic_bag_free(tree->diagnostics);
    expression_syntax_free(tree->root);
    syntax_token_free(tree->end_of_file_token);
    source_text_free(tree->source);
    free(tree);
}

DiagnosticBag* syntax_tree_take_diagnostics(SyntaxTree* tree) {
    DiagnosticBag* diagnostics = tree->diagnostics;
    tree->diagnostics = NULL;
    return diagnostics;
}

SyntaxTree* syntax_tree_parse(str text) {
    return syntax_tree_parse_source(source_text_from(text));
}

SyntaxTokenBuf syntax_tree_parse_tokens(str text) {
    return syntax_tree_parse_tokens_source(source_text_from(text));
}

void syntax_token_buf_free(SyntaxTokenBuf buf) {
    for (uint64_t i = 0; i < buf.len; i++) {
        syntax_token_free(buf.ptr[i]);
    }
    BUF_FREE(buf);
}

static SyntaxTree* syntax_tree_parse_source(SourceText source) {
    Parser* parser = parser_new(source);
    SyntaxTree* tree = parser_parse(parser);
    parser_free(parser);
    return tree;
}

static SyntaxTokenBuf syntax_tree_parse_tokens_source(SourceText source) {
    Lexer* lexer = lexer_new(source);
    SyntaxTokenBuf buf = BUF_NEW;
    while (true) {
        SyntaxToken* token = lexer_next_token(lexer);
        if (token->kind == SYNTAX_KIND_END_OF_FILE_TOKEN) {
            syntax_token_free(token);
            break;
        }
        BUF_PUSH(&buf, token);
    }

    lexer_free(lexer);
    source_text_free(source);
    return buf;
}
