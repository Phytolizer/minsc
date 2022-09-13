#include "syntax_node.h"

#include <stdlib.h>

#include "expression_syntax.h"

SyntaxKind syntax_node_kind(const SyntaxNode* node) {
    switch (node->type) {
        case SYNTAX_NODE_TYPE_EXPRESSION:
            return expression_syntax_kind((const ExpressionSyntax*)node);
    }

    abort();
}
