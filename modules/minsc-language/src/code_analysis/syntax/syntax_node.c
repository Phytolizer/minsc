#include "minsc/code_analysis/syntax/syntax_node.h"

#include <buf/buf.h>
#include <println/println.h>
#include <stdbool.h>
#include <stdio.h>
#include <str/str.h>

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/runtime/object.h"
#include "minsc/support/minsc_assert.h"

static void pretty_print(const SyntaxNode* root, str indent, bool is_last, FILE* f);

SyntaxKind syntax_node_kind(const SyntaxNode* node) {
    switch (node->type) {
        case SYNTAX_NODE_TYPE_TOKEN:
            return ((const SyntaxToken*)node)->kind;
        case SYNTAX_NODE_TYPE_EXPRESSION:
            return expression_syntax_kind((const ExpressionSyntax*)node);
    }

    MINSC_ABORT("Invalid syntax node type");
}

SyntaxNodeChildren syntax_node_children(const SyntaxNode* node) {
    switch (node->type) {
        case SYNTAX_NODE_TYPE_TOKEN:
            // no children
            return (SyntaxNodeChildren)BUF_NEW;
        case SYNTAX_NODE_TYPE_EXPRESSION:
            return expression_syntax_children((const ExpressionSyntax*)node);
    }

    MINSC_ABORT("Invalid syntax node type");
}

TextSpan syntax_node_span(const SyntaxNode* node) {
    if (node->type == SYNTAX_NODE_TYPE_TOKEN) {
        return syntax_token_span((const SyntaxToken*)node);
    }

    SyntaxNodeChildren children = syntax_node_children(node);
    MINSC_ASSERT(children.len > 0);

    TextSpan first = syntax_node_span(children.ptr[0]);
    TextSpan last = syntax_node_span(BUF_LAST(children));
    return text_span_from_bounds(first.start, text_span_end(last));
}

void syntax_node_pretty_print(const SyntaxNode* root, FILE* f) {
    pretty_print(root, str_null, true, f);
}

static void pretty_print(const SyntaxNode* root, str indent, bool is_last, FILE* f) {
    str marker = is_last ? str_lit("└───") : str_lit("├───");
    printf(
        str_fmt str_fmt str_fmt,
        str_arg(indent),
        str_arg(marker),
        str_arg(syntax_kind_string(syntax_node_kind(root)))
    );
    if (root->type == SYNTAX_NODE_TYPE_TOKEN && ((SyntaxToken*)root)->value != NULL) {
        str value = object_string(((SyntaxToken*)root)->value);
        printf(" " str_fmt, str_arg(value));
        str_free(value);
    }
    fprintln(f);

    str new_indent = str_null;
    str_cat(&new_indent, indent, is_last ? str_lit("    ") : str_lit("│   "));

    SyntaxNodeChildren children = syntax_node_children(root);
    for (size_t i = 0; i < children.len; i++) {
        pretty_print(children.ptr[i], new_indent, i == children.len - 1, f);
    }
    str_free(new_indent);
    BUF_FREE(children);
}
