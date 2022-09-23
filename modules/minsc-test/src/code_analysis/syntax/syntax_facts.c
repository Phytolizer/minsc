#include "minsc_test/code_analysis/syntax/syntax_facts.h"

#include <minsc/code_analysis/syntax/syntax_facts.h>
#include <minsc/code_analysis/syntax/syntax_kind.h>
#include <minsc/code_analysis/syntax/syntax_token.h>
#include <minsc/code_analysis/syntax/syntax_tree.h>
#include <str/str.h>
#include <test/test.h>

static TEST_FUNC(state, get_text_round_trips, SyntaxKind kind) {
    str text = syntax_facts_get_text(kind);

    SyntaxTokenBuf tokens = syntax_tree_parse_tokens(text);
    TEST_ASSERT(
        state,
        tokens.len == 1,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected 1 token, got %zu",
        tokens.len
    );
    TEST_ASSERT(
        state,
        tokens.ptr[0]->kind == kind,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected token kind " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(kind)),
        str_arg(syntax_kind_string(tokens.ptr[0]->kind))
    );
    TEST_ASSERT(
        state,
        str_eq(tokens.ptr[0]->text, text),
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected token text " str_fmt ", got " str_fmt,
        str_arg(text),
        str_arg(tokens.ptr[0]->text)
    );
    syntax_token_buf_free(tokens);
    PASS();
}

SUITE_FUNC(state, syntax_facts) {
    for (SyntaxKind kind = SYNTAX_KIND_ZERO; kind < SYNTAX_KIND_COUNT; kind++) {
        str text = syntax_facts_get_text(kind);
        if (text.len == 0) {
            continue;
        }
        RUN_TEST(
            state,
            get_text_round_trips,
            str_printf("round trip: " str_fmt, str_arg(syntax_kind_string(kind))),
            kind
        );
    }
}
