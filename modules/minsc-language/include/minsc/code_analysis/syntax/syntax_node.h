#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_NODE_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_NODE_H_

#include <buf/buf.h>

#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/text_span.h"

typedef enum {
#define X(x) SYNTAX_NODE_TYPE_##x,
#include "minsc/code_analysis/syntax/syntax_node_type.inc"
#undef X
} SyntaxNodeType;

typedef struct SyntaxNode {
    SyntaxNodeType type;
} SyntaxNode;

typedef BUF(const SyntaxNode*) SyntaxNodeChildren;

SyntaxKind syntax_node_kind(const SyntaxNode* node);
SyntaxNodeChildren syntax_node_children(const SyntaxNode* node);
TextSpan syntax_node_span(const SyntaxNode* node);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_NODE_H_
