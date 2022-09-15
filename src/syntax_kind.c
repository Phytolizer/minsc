#include "syntax_kind.h"

#include <str/str.h>

static const char* const SYNTAX_KIND_STRINGS[] = {
#define X(x) #x,
#include "syntax_kind_uppercamel.inc"
#undef X
};

extern str syntax_kind_string(SyntaxKind kind) {
    switch (kind) {
#define X(x) case SYNTAX_KIND_##x:
#include "syntax_kind.inc"
#undef X
        return str_ref(SYNTAX_KIND_STRINGS[kind]);
        default:
            return str_lit("SYNTAX_KIND_UNKNOWN");
    }
}
