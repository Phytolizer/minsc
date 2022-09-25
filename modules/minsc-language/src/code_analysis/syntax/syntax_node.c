#include "minsc/code_analysis/syntax/syntax_node.h"

#include <buf/buf.h>

#include "minsc/code_analysis/syntax/expression_syntax.h"
#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_token.h"
#include "minsc/support/minsc_assert.h"

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
