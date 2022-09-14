#ifndef MINSC_TOKEN_H_
#define MINSC_TOKEN_H_

#include <stddef.h>
#include <str/str.h>

#include "object.h"
#include "syntax_kind.h"
#include "syntax_node.h"

typedef struct SyntaxToken {
    SyntaxNode base;
    SyntaxKind kind;
    size_t position;
    str text;
    Object* value;
} SyntaxToken;

SyntaxToken* syntax_token_new(SyntaxKind kind, size_t position, str text, Object* value);
SyntaxToken* syntax_token_dup(const SyntaxToken* token);
void syntax_token_free(SyntaxToken* token);

#endif  // MINSC_TOKEN_H_
