#ifndef MINSC_TOKEN_H_
#define MINSC_TOKEN_H_

#include <stddef.h>
#include <str/str.h>

#include "object.h"
#include "syntax_kind.h"

typedef struct SyntaxToken {
    SyntaxKind kind;
    size_t position;
    str text;
    Object* value;
} SyntaxToken;

void syntax_token_free(SyntaxToken token);

#endif  // MINSC_TOKEN_H_
