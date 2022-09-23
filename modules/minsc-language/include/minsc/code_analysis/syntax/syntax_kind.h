#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_KIND_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_KIND_H_

#include <str/str.h>

typedef enum {
#define X(x) SYNTAX_KIND_##x,
#include "minsc/code_analysis/syntax/syntax_kind.inc"
#undef X
    SYNTAX_KIND_COUNT,
    SYNTAX_KIND_ZERO = SYNTAX_KIND_BAD_TOKEN,
} SyntaxKind;

str syntax_kind_string(SyntaxKind kind);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_KIND_H_
