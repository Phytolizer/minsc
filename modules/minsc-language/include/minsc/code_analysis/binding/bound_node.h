#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_NODE_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_NODE_H_

typedef enum {
#define X(x) BOUND_NODE_TYPE_##x,
#include "minsc/code_analysis/binding/bound_node_type.inc"
#undef X
} BoundNodeType;

typedef enum {
#define X(x) BOUND_NODE_KIND_##x,
#include "minsc/code_analysis/binding/bound_node_kind.inc"
#undef X
} BoundNodeKind;

typedef struct BoundNode {
    BoundNodeType type;
} BoundNode;

extern void bound_node_free(BoundNode* node);
extern BoundNodeKind bound_node_kind(const BoundNode* node);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_NODE_H_
