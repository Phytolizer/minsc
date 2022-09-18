#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_FACTS_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_FACTS_H_

#include <stddef.h>
#include <str/str.h>

#include "minsc/code_analysis/syntax/syntax_kind.h"

size_t binary_operator_precedence(SyntaxKind kind);
size_t unary_operator_precedence(SyntaxKind kind);

SyntaxKind keyword_kind(str text);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_FACTS_H_
