#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_KIND_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_KIND_H_

#include <str/str.h>

typedef enum {
#define X(x) SYNTAX_KIND_##x,
#include "syntax_kind.inc"
#undef X
} SyntaxKind;

extern str syntax_kind_string(SyntaxKind kind);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_KIND_H_
