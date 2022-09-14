#include "syntax_node.h"

#include <stdlib.h>

#include "buf/buf.h"
#include "expression_syntax.h"
#include "syntax_token.h"

SyntaxKind syntax_node_kind(const SyntaxNode* node) {
    switch (node->type) {
        case SYNTAX_NODE_TYPE_TOKEN:
            return ((const SyntaxToken*)node)->kind;
        case SYNTAX_NODE_TYPE_EXPRESSION:
            return expression_syntax_kind((const ExpressionSyntax*)node);
    }

    abort();
}

SyntaxNodeChildren syntax_node_children(const SyntaxNode* node) {
    switch (node->type) {
        case SYNTAX_NODE_TYPE_TOKEN:
            // no children
            return (SyntaxNodeChildren)BUF_NEW;
        case SYNTAX_NODE_TYPE_EXPRESSION:
            return expression_syntax_children((const ExpressionSyntax*)node);
    }

    abort();
}
