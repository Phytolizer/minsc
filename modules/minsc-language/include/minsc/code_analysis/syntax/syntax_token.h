#ifndef MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TOKEN_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TOKEN_H_

#include <stdbool.h>
#include <stddef.h>
#include <str/str.h>

#include "minsc/code_analysis/syntax/syntax_kind.h"
#include "minsc/code_analysis/syntax/syntax_node.h"
#include "minsc/runtime/object.h"

typedef struct SyntaxToken {
    SyntaxNode base;
    SyntaxKind kind;
    size_t position;
    str text;
    Object* value;
    // dup does nothing on manufactured tokens
    bool manufactured;
} SyntaxToken;

SyntaxToken* syntax_token_new(SyntaxKind kind, size_t position, str text, Object* value);
SyntaxToken*
syntax_token_new_manufactured(SyntaxKind kind, size_t position, str text, Object* value);
SyntaxToken* syntax_token_dup(SyntaxToken* token);
void syntax_token_free(SyntaxToken* token);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_SYNTAX_TOKEN_H_
