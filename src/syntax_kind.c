#include "minsc/code_analysis/syntax_kind.h"

#include <str/str.h>

static const char* const SYNTAX_KIND_STRINGS[] = {
#define X(x) #x,
#include "minsc/code_analysis/syntax_kind_uppercamel.inc"
#undef X
};

static const size_t SYNTAX_KIND_STRING_LENGTHS[] = {
#define X(x) sizeof(#x) - 1,
#include "minsc/code_analysis/syntax_kind_uppercamel.inc"
#undef X
};

extern str syntax_kind_string(SyntaxKind kind) {
    switch (kind) {
#define X(x) case SYNTAX_KIND_##x:
#include "minsc/code_analysis/syntax_kind.inc"
#undef X
        return str_ref_chars(SYNTAX_KIND_STRINGS[kind],
                             SYNTAX_KIND_STRING_LENGTHS[kind]);
        default:
            return str_lit("SYNTAX_KIND_UNKNOWN");
    }
}
