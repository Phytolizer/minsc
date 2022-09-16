#include "minsc/code_analysis/syntax/syntax_facts.h"

extern size_t binary_operator_precedence(SyntaxKind kind) {
    switch (kind) {
        case SYNTAX_KIND_STAR_TOKEN:
        case SYNTAX_KIND_SLASH_TOKEN:
            return 2;
        case SYNTAX_KIND_PLUS_TOKEN:
        case SYNTAX_KIND_MINUS_TOKEN:
            return 1;
        default:
            return 0;
    }
}

extern size_t unary_operator_precedence(SyntaxKind kind) {
    switch (kind) {
        case SYNTAX_KIND_PLUS_TOKEN:
        case SYNTAX_KIND_MINUS_TOKEN:
            return 3;
        default:
            return 0;
    }
}
