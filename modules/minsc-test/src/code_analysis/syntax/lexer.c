#include "minsc_test/code_analysis/syntax/lexer.h"

#include <buf/buf.h>
#include <minsc/code_analysis/syntax/syntax_tree.h>

#include "minsc_test/util/str_esc.h"

typedef struct {
    SyntaxKind kind;
    str text;
} TestToken;

typedef BUF(const TestToken) TestTokenBuf;

static TestTokenBuf get_tokens(void) {
    static const TestToken a[] = {
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c(" ")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("  ")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("\r")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("\n")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("\r\n")},
        {SYNTAX_KIND_NUMBER_TOKEN, str_lit_c("1")},
        {SYNTAX_KIND_NUMBER_TOKEN, str_lit_c("123")},
        {SYNTAX_KIND_PLUS_TOKEN, str_lit_c("+")},
        {SYNTAX_KIND_MINUS_TOKEN, str_lit_c("-")},
        {SYNTAX_KIND_STAR_TOKEN, str_lit_c("*")},
        {SYNTAX_KIND_SLASH_TOKEN, str_lit_c("/")},
        {SYNTAX_KIND_BANG_TOKEN, str_lit_c("!")},
        {SYNTAX_KIND_EQUALS_TOKEN, str_lit_c("=")},
        {SYNTAX_KIND_AMPERSAND_AMPERSAND_TOKEN, str_lit_c("&&")},
        {SYNTAX_KIND_PIPE_PIPE_TOKEN, str_lit_c("||")},
        {SYNTAX_KIND_EQUALS_EQUALS_TOKEN, str_lit_c("==")},
        {SYNTAX_KIND_BANG_EQUALS_TOKEN, str_lit_c("!=")},
        {SYNTAX_KIND_OPEN_PARENTHESIS_TOKEN, str_lit_c("(")},
        {SYNTAX_KIND_CLOSE_PARENTHESIS_TOKEN, str_lit_c(")")},
        {SYNTAX_KIND_IDENTIFIER_TOKEN, str_lit_c("a")},
        {SYNTAX_KIND_IDENTIFIER_TOKEN, str_lit_c("abc")},
        {SYNTAX_KIND_FALSE_KEYWORD, str_lit_c("false")},
        {SYNTAX_KIND_TRUE_KEYWORD, str_lit_c("true")},
    };
    return (TestTokenBuf)BUF_ARRAY(a);
}

static TEST_FUNC(state, lexes_token, TestToken token) {
    SyntaxTokenBuf tokens = syntax_tree_parse_tokens(token.text);

    TEST_ASSERT(
        state,
        tokens.len == 1,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected 1 token, got %zu",
        tokens.len
    );
    SyntaxToken* tok = tokens.ptr[0];
    TEST_ASSERT(
        state,
        tok->kind == token.kind,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(token.kind)),
        str_arg(syntax_kind_string(tok->kind))
    );
    TEST_ASSERT(
        state,
        str_eq(tok->text, token.text),
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(token.text),
        str_arg(tok->text)
    );
    syntax_token_buf_free(tokens);
    PASS();
}

SUITE_FUNC(state, lexer) {
    TestTokenBuf test_tokens = get_tokens();
    for (uint64_t i = 0; i < test_tokens.len; i++) {
        str text_esc = str_esc(test_tokens.ptr[i].text);
        RUN_TEST(
            state,
            lexes_token,
            str_printf(
                "Lexes token (" str_fmt ", '" str_fmt "')",
                str_arg(syntax_kind_string(test_tokens.ptr[i].kind)),
                str_arg(text_esc)
            ),
            test_tokens.ptr[i]
        );
        str_free(text_esc);
    }
}
