#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_FACTS_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_FACTS_H_

#include <stddef.h>
#include <str/str.h>

#include "syntax_kind.h"

extern size_t binary_operator_precedence(SyntaxKind kind);
extern size_t unary_operator_precedence(SyntaxKind kind);

extern SyntaxKind keyword_kind(str text);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_FACTS_H_
