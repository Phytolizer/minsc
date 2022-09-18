#include "minsc/code_analysis/binding/bound_node.h"

#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/support/minsc_assert.h"

void bound_node_free(BoundNode* node) {
    switch (node->type) {
        case BOUND_NODE_TYPE_EXPRESSION:
            bound_expression_free((BoundExpression*)node);
            break;
    }

    MINSC_ABORT("Invalid bound node type");
}

BoundNodeKind bound_node_kind(const BoundNode* node) {
    switch (node->type) {
        case BOUND_NODE_TYPE_EXPRESSION:
            return bound_expression_kind((const BoundExpression*)node);
    }

    MINSC_ABORT("Invalid bound node type");
}
