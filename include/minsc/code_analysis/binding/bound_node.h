#ifndef MINSC_BOUND_NODE_H_
#define MINSC_BOUND_NODE_H_

typedef enum {
#define X(x) BOUND_NODE_TYPE_##x,
#include "bound_node_type.inc"
#undef X
} BoundNodeType;

typedef enum {
#define X(x) BOUND_NODE_KIND_##x,
#include "bound_node_kind.inc"
#undef X
} BoundNodeKind;

typedef struct BoundNode {
    BoundNodeType type;
} BoundNode;

extern void bound_node_free(BoundNode* node);
extern BoundNodeKind bound_node_kind(const BoundNode* node);

#endif  // MINSC_BOUND_NODE_H_
