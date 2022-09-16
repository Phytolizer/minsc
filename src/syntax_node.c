#include "minsc/code_analysis/syntax_node.h"

#include <buf/buf.h>
#include <stdlib.h>

#include "minsc/code_analysis/expression_syntax.h"
#include "minsc/code_analysis/syntax_token.h"
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
