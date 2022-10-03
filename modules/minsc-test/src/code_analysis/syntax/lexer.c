#include "minsc_test/code_analysis/syntax/lexer.h"

#include <buf/buf.h>
#include <minsc/code_analysis/syntax/syntax_facts.h>
#include <minsc/code_analysis/syntax/syntax_kind.h>
#include <minsc/code_analysis/syntax/syntax_token.h>
#include <minsc/code_analysis/syntax/syntax_tree.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <str/str.h>
#include <test/test.h>
#include <uthash.h>

#include "minsc_test/util/str_esc.h"

typedef struct {
    SyntaxKind kind;
    str text;
} TestToken;

typedef BUF(TestToken) TestTokenBuf;

static TestTokenBuf get_tokens(void) {
    TestTokenBuf tokens = BUF_NEW;
    for (SyntaxKind kind = SYNTAX_KIND_ZERO; kind < SYNTAX_KIND_COUNT; kind++) {
        str text = syntax_facts_get_text(kind);
        if (text.len > 0) {
            TestToken token = {kind, text};
            BUF_PUSH(&tokens, token);
        }
    }
    static TestToken a[] = {
        {SYNTAX_KIND_IDENTIFIER_TOKEN, str_lit_c("a")},
        {SYNTAX_KIND_IDENTIFIER_TOKEN, str_lit_c("abc")},
        {SYNTAX_KIND_NUMBER_TOKEN, str_lit_c("1")},
        {SYNTAX_KIND_NUMBER_TOKEN, str_lit_c("123")},
    };
    TestTokenBuf dynamic_tokens = BUF_ARRAY(a);
    BUF_CONCAT(&tokens, dynamic_tokens);
    return tokens;
}

static TestTokenBuf get_separators(void) {
    static TestToken a[] = {
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c(" ")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("  ")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("\r")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("\n")},
        {SYNTAX_KIND_WHITESPACE_TOKEN, str_lit_c("\r\n")},
    };
    return (TestTokenBuf)BUF_ARRAY(a);
}

static TestTokenBuf get_all_tokens(void) {
    TestTokenBuf buf = BUF_NEW;
    TestTokenBuf tokens = get_tokens();
    BUF_CONCAT(&buf, tokens);
    BUF_FREE(tokens);
    TestTokenBuf separators = get_separators();
    BUF_CONCAT(&buf, separators);
    return buf;
}

typedef struct {
    str key;
    UT_hash_handle hh;
} SyntaxKindSetEntry;

static void syntax_kind_set_clear(SyntaxKindSetEntry* set) {
    SyntaxKindSetEntry* entry;
    SyntaxKindSetEntry* tmp;
    HASH_ITER(hh, set, entry, tmp) {
        HASH_DEL(set, entry);
        free(entry);
    }
}

static TEST_FUNC0(state, lexer_tests_all_tokens) {
    SyntaxKindSetEntry* token_kinds = NULL;
    for (SyntaxKind kind = SYNTAX_KIND_ZERO; kind < SYNTAX_KIND_COUNT; kind++) {
        str s = syntax_kind_string(kind);
        if (str_has_suffix(s, str_lit("Token")) || str_has_suffix(s, str_lit("Keyword"))) {
            SyntaxKindSetEntry* entry = malloc(sizeof(SyntaxKindSetEntry));
            entry->key = s;
            // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
            HASH_ADD_KEYPTR(hh, token_kinds, s.ptr, s.len, entry);
        }
    }

    TestTokenBuf tested_tokens = get_all_tokens();
    for (uint64_t i = 0; i < tested_tokens.len; i++) {
        TestToken token = tested_tokens.ptr[i];
        SyntaxKindSetEntry* entry;
        str s = syntax_kind_string(token.kind);
        // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
        HASH_FIND(hh, token_kinds, s.ptr, s.len, entry);
        if (entry != NULL) {
            HASH_DEL(token_kinds, entry);
            free(entry);
        }
    }
    BUF_FREE(tested_tokens);

    {
        SyntaxKindSetEntry* entry;
        static const str acceptable_tokens[] = {
            str_lit_c("EndOfFileToken"),
            str_lit_c("BadToken"),
        };
        for (size_t i = 0; i < sizeof(acceptable_tokens) / sizeof(acceptable_tokens[0]); i++) {
            str s = acceptable_tokens[i];
            // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
            HASH_FIND(hh, token_kinds, s.ptr, s.len, entry);
            if (entry != NULL) {
                HASH_DEL(token_kinds, entry);
                free(entry);
            }
        }
    }

    if (token_kinds != NULL) {
        str message = str_lit("The following tokens were not tested:\n");
        SyntaxKindSetEntry* el;
        SyntaxKindSetEntry* tmp;
        HASH_ITER(hh, token_kinds, el, tmp) {
            str_append(&message, str_lit("    "));
            str_append(&message, str_ref(el->key));
            str_append(&message, str_lit("\n"));
            HASH_DEL(token_kinds, el);
            free(el);
        }
        FAIL(state, CLEANUP(str_free(message)), str_fmt, str_arg(message));
    }

    PASS();
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

typedef struct TestTokenPair {
    TestToken tokens[2];
} TestTokenPair;

typedef BUF(TestTokenPair) TestTokenPairBuf;

static bool requires_separator(TestTokenPair pair) {
    bool result = false;

    bool t0_is_keyword =
        str_has_suffix(syntax_kind_string(pair.tokens[0].kind), str_lit("Keyword"));
    bool t1_is_keyword =
        str_has_suffix(syntax_kind_string(pair.tokens[1].kind), str_lit("Keyword"));

    if (pair.tokens[0].kind == SYNTAX_KIND_NUMBER_TOKEN) {
        result = pair.tokens[1].kind == SYNTAX_KIND_NUMBER_TOKEN;
    } else if (pair.tokens[0].kind == SYNTAX_KIND_IDENTIFIER_TOKEN || t0_is_keyword) {
        result = pair.tokens[1].kind == SYNTAX_KIND_IDENTIFIER_TOKEN ||
                 pair.tokens[1].kind == SYNTAX_KIND_NUMBER_TOKEN || t1_is_keyword;
    } else if (pair.tokens[0].kind == SYNTAX_KIND_EQUALS_TOKEN || pair.tokens[0].kind == SYNTAX_KIND_BANG_TOKEN) {
        result = pair.tokens[1].kind == SYNTAX_KIND_EQUALS_TOKEN ||
                 pair.tokens[1].kind == SYNTAX_KIND_EQUALS_EQUALS_TOKEN;
    }

    return result;
}

static TestTokenPairBuf get_token_pairs(void) {
    TestTokenBuf tokens = get_tokens();

    TestTokenPairBuf pairs = BUF_NEW;

    for (size_t i = 0; i < tokens.len; i++) {
        for (size_t j = 0; j < tokens.len; j++) {
            TestTokenPair pair = {
                .tokens = {tokens.ptr[i], tokens.ptr[j]},
            };
            if (!requires_separator(pair)) {
                BUF_PUSH(&pairs, pair);
            }
        }
    }
    BUF_FREE(tokens);

    return pairs;
}

static TEST_FUNC(state, lexes_token_pair, TestTokenPair pair) {
    str text = str_cat_ret(pair.tokens[0].text, pair.tokens[1].text);
    SyntaxTokenBuf tokens = syntax_tree_parse_tokens(text);

    TEST_ASSERT(
        state,
        tokens.len == 2,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected 2 tokens, got %zu",
        tokens.len
    );
    SyntaxToken* tok0 = tokens.ptr[0];
    SyntaxToken* tok1 = tokens.ptr[1];
    TEST_ASSERT(
        state,
        tok0->kind == pair.tokens[0].kind,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(pair.tokens[0].kind)),
        str_arg(syntax_kind_string(tok0->kind))
    );
    TEST_ASSERT(
        state,
        tok1->kind == pair.tokens[1].kind,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(pair.tokens[1].kind)),
        str_arg(syntax_kind_string(tok1->kind))
    );
    TEST_ASSERT(
        state,
        str_eq(tok0->text, pair.tokens[0].text),
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(pair.tokens[0].text),
        str_arg(tok0->text)
    );
    TEST_ASSERT(
        state,
        str_eq(tok1->text, pair.tokens[1].text),
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(pair.tokens[1].text),
        str_arg(tok1->text)
    );
    syntax_token_buf_free(tokens);
    PASS();
}

typedef struct TestTokenPairWithSeparator {
    TestTokenPair pair;
    TestToken separator;
} TestTokenPairWithSeparator;

typedef BUF(TestTokenPairWithSeparator) TestTokenPairWithSeparatorBuf;

static TestTokenPairWithSeparatorBuf get_token_pairs_with_separators(void) {
    TestTokenBuf tokens = get_tokens();
    TestTokenBuf separators = get_separators();

    TestTokenPairWithSeparatorBuf pairs_with_separators = BUF_NEW;

    for (size_t i = 0; i < tokens.len; i++) {
        for (size_t j = 0; j < tokens.len; j++) {
            TestTokenPair pair = {{tokens.ptr[i], tokens.ptr[j]}};
            if (requires_separator(pair)) {
                for (size_t k = 0; k < separators.len; k++) {
                    TestTokenPairWithSeparator pair_with_separator = {
                        .pair = pair,
                        .separator = separators.ptr[k],
                    };
                    BUF_PUSH(&pairs_with_separators, pair_with_separator);
                }
            }
        }
    }
    BUF_FREE(tokens);

    return pairs_with_separators;
}

static TEST_FUNC(state, lexes_token_pair_with_separator, TestTokenPairWithSeparator pair) {
    str text_tmp = str_cat_ret(pair.pair.tokens[0].text, pair.separator.text);
    str text = str_cat_ret(text_tmp, pair.pair.tokens[1].text);
    str_free(text_tmp);
    SyntaxTokenBuf tokens = syntax_tree_parse_tokens(text);

    TEST_ASSERT(
        state,
        tokens.len == 3,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected 3 tokens, got %zu",
        tokens.len
    );
    SyntaxToken* tok0 = tokens.ptr[0];
    SyntaxToken* tok1 = tokens.ptr[1];
    SyntaxToken* tok2 = tokens.ptr[2];
    TEST_ASSERT(
        state,
        tok0->kind == pair.pair.tokens[0].kind,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(pair.pair.tokens[0].kind)),
        str_arg(syntax_kind_string(tok0->kind))
    );
    TEST_ASSERT(
        state,
        tok1->kind == pair.separator.kind,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(pair.separator.kind)),
        str_arg(syntax_kind_string(tok1->kind))
    );
    TEST_ASSERT(
        state,
        tok2->kind == pair.pair.tokens[1].kind,
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(pair.pair.tokens[1].kind)),
        str_arg(syntax_kind_string(tok2->kind))
    );
    TEST_ASSERT(
        state,
        str_eq(tok0->text, pair.pair.tokens[0].text),
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(pair.pair.tokens[0].text),
        str_arg(tok0->text)
    );
    TEST_ASSERT(
        state,
        str_eq(tok1->text, pair.separator.text),
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(pair.separator.text),
        str_arg(tok1->text)
    );
    TEST_ASSERT(
        state,
        str_eq(tok2->text, pair.pair.tokens[1].text),
        CLEANUP(syntax_token_buf_free(tokens)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(pair.pair.tokens[1].text),
        str_arg(tok2->text)
    );
    syntax_token_buf_free(tokens);
    PASS();
}

SUITE_FUNC(state, lexer) {
    TestTokenBuf test_tokens = get_all_tokens();
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
    BUF_FREE(test_tokens);

    TestTokenPairBuf test_token_pairs = get_token_pairs();
    for (uint64_t i = 0; i < test_token_pairs.len; i++) {
        str text_esc0 = str_esc(test_token_pairs.ptr[i].tokens[0].text);
        str text_esc1 = str_esc(test_token_pairs.ptr[i].tokens[1].text);
        RUN_TEST(
            state,
            lexes_token_pair,
            str_printf(
                "Lexes token pair (" str_fmt ", '" str_fmt "'), (" str_fmt ", '" str_fmt "')",
                str_arg(syntax_kind_string(test_token_pairs.ptr[i].tokens[0].kind)),
                str_arg(text_esc0),
                str_arg(syntax_kind_string(test_token_pairs.ptr[i].tokens[1].kind)),
                str_arg(text_esc1)
            ),
            test_token_pairs.ptr[i]
        );
        str_free(text_esc0);
        str_free(text_esc1);
    }
    BUF_FREE(test_token_pairs);

    TestTokenPairWithSeparatorBuf test_token_pairs_with_separator =
        get_token_pairs_with_separators();
    for (uint64_t i = 0; i < test_token_pairs_with_separator.len; i++) {
        str text_esc0 = str_esc(test_token_pairs_with_separator.ptr[i].pair.tokens[0].text);
        str text_esc1 = str_esc(test_token_pairs_with_separator.ptr[i].pair.tokens[1].text);
        str text_esc_sep = str_esc(test_token_pairs_with_separator.ptr[i].separator.text);
        RUN_TEST(
            state,
            lexes_token_pair_with_separator,
            str_printf(
                "Lexes token pair (" str_fmt ", '" str_fmt "'), (" str_fmt ", '" str_fmt "')"
                " with separator (" str_fmt ", '" str_fmt "')",
                str_arg(
                    syntax_kind_string(test_token_pairs_with_separator.ptr[i].pair.tokens[0].kind)
                ),
                str_arg(text_esc0),
                str_arg(
                    syntax_kind_string(test_token_pairs_with_separator.ptr[i].pair.tokens[1].kind)
                ),
                str_arg(text_esc1),
                str_arg(syntax_kind_string(test_token_pairs_with_separator.ptr[i].separator.kind)),
                str_arg(text_esc_sep)
            ),
            test_token_pairs_with_separator.ptr[i]
        );
        str_free(text_esc0);
        str_free(text_esc1);
        str_free(text_esc_sep);
    }
    BUF_FREE(test_token_pairs_with_separator);

    RUN_TEST0(state, lexer_tests_all_tokens, str_lit("Lexer tests all tokens"));
}
