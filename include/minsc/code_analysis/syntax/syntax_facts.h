#ifndef MINSC_SYNTAX_FACTS_H_
#define MINSC_SYNTAX_FACTS_H_

#include <stddef.h>

#include "syntax_kind.h"

extern size_t binary_operator_precedence(SyntaxKind kind);
extern size_t unary_operator_precedence(SyntaxKind kind);

#endif  // MINSC_SYNTAX_FACTS_H_
