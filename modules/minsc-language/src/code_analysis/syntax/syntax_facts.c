#include "minsc/code_analysis/syntax/syntax_facts.h"

#include <buf/buf.h>
#include <str/str.h>

#include "minsc/code_analysis/syntax/syntax_kind.h"

size_t binary_operator_precedence(SyntaxKind kind) {
    switch (kind) {
        case SYNTAX_KIND_STAR_TOKEN:
        case SYNTAX_KIND_SLASH_TOKEN:
            return 5;
        case SYNTAX_KIND_PLUS_TOKEN:
        case SYNTAX_KIND_MINUS_TOKEN:
            return 4;
        case SYNTAX_KIND_EQUALS_EQUALS_TOKEN:
        case SYNTAX_KIND_BANG_EQUALS_TOKEN:
            return 3;
        case SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN:
            return 2;
        case SYNTAX_KIND_PIPE_PIPE_TOKEN:
            return 1;
        default:
            return 0;
    }
}

size_t unary_operator_precedence(SyntaxKind kind) {
    switch (kind) {
        case SYNTAX_KIND_PLUS_TOKEN:
        case SYNTAX_KIND_MINUS_TOKEN:
        case SYNTAX_KIND_BANG_TOKEN:
            return 6;
        default:
            return 0;
    }
}

SyntaxKind keyword_kind(str text) {
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

str syntax_facts_get_text(SyntaxKind kind) {
    switch (kind) {
        case SYNTAX_KIND_PLUS_TOKEN:
            return str_lit("+");
        case SYNTAX_KIND_MINUS_TOKEN:
            return str_lit("-");
        case SYNTAX_KIND_STAR_TOKEN:
            return str_lit("*");
        case SYNTAX_KIND_SLASH_TOKEN:
            return str_lit("/");
        case SYNTAX_KIND_BANG_TOKEN:
            return str_lit("!");
        case SYNTAX_KIND_EQUALS_EQUALS_TOKEN:
            return str_lit("==");
        case SYNTAX_KIND_BANG_EQUALS_TOKEN:
            return str_lit("!=");
        case SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN:
            return str_lit("&&");
        case SYNTAX_KIND_PIPE_PIPE_TOKEN:
            return str_lit("||");
        case SYNTAX_KIND_EQUALS_TOKEN:
            return str_lit("=");
        case SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN:
            return str_lit("(");
        case SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN:
            return str_lit(")");
        case SYNTAX_KIND_FALSE_KEYWORD:
            return str_lit("false");
        case SYNTAX_KIND_TRUE_KEYWORD:
            return str_lit("true");
        default:
            return str_null;
    }
}

SyntaxKindBuf syntax_facts_get_binary_operator_kinds(void) {
    SyntaxKindBuf buf = BUF_NEW;
    for (SyntaxKind kind = SYNTAX_KIND_ZERO; kind < SYNTAX_KIND_COUNT; kind++) {
        if (binary_operator_precedence(kind) > 0) {
            BUF_PUSH(&buf, kind);
        }
    }
    return buf;
}

SyntaxKindBuf syntax_facts_get_unary_operator_kinds(void) {
    SyntaxKindBuf buf = BUF_NEW;
    for (SyntaxKind kind = SYNTAX_KIND_ZERO; kind < SYNTAX_KIND_COUNT; kind++) {
        if (unary_operator_precedence(kind) > 0) {
            BUF_PUSH(&buf, kind);
        }
    }
    return buf;
}
