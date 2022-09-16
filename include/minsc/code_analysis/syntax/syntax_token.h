#ifndef MINSC_TOKEN_H_
#define MINSC_TOKEN_H_

#include <stdbool.h>
#include <stddef.h>
#include <str/str.h>

#include "minsc/runtime/object.h"
#include "syntax_kind.h"
#include "syntax_node.h"

typedef struct SyntaxToken {
    SyntaxNode base;
    SyntaxKind kind;
    size_t position;
    str text;
    Object* value;
    // dup does nothing on manufactured tokens
    bool manufactured;
} SyntaxToken;

SyntaxToken* syntax_token_new(SyntaxKind kind,
                              size_t position,
                              str text,
                              Object* value);
SyntaxToken* syntax_token_new_manufactured(SyntaxKind kind,
                                           size_t position,
                                           str text,
                                           Object* value);
SyntaxToken* syntax_token_dup(SyntaxToken* token);
void syntax_token_free(SyntaxToken* token);

#endif  // MINSC_TOKEN_H_