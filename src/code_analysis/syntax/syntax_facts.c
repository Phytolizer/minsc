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

extern SyntaxKind keyword_kind(str text) {
    SyntaxKind kind;

    if (str_eq(text, str_lit("true"))) {
        kind = SYNTAX_KIND_TRUE_KEYWORD;
    } else if (str_eq(text, str_lit("false"))) {
        kind = SYNTAX_KIND_FALSE_KEYWORD;
    } else {
        kind = SYNTAX_KIND_IDENTIFIER_TOKEN;
    }

    return kind;
}
