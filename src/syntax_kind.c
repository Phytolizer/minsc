#include "syntax_kind.h"

extern str syntax_kind_string(SyntaxKind kind) {
    switch (kind) {
#define X(x) \
    case SYNTAX_KIND_##x: \
        return str_lit(#x);
#include "syntax_kind.inc"
#undef X
    }
    return str_lit("SYNTAX_KIND_UNKNOWN");
}
