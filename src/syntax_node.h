#ifndef MINSC_SYNTAX_NODE_H_
#define MINSC_SYNTAX_NODE_H_

#include "syntax_kind.h"

typedef enum {
#define X(x) SYNTAX_NODE_TYPE_##x,
#include "syntax_node_type.inc"
#undef X
} SyntaxNodeType;

typedef struct SyntaxNode {
    SyntaxNodeType type;
} SyntaxNode;

SyntaxKind syntax_node_kind(const SyntaxNode* node);

#endif  // MINSC_SYNTAX_NODE_H_
