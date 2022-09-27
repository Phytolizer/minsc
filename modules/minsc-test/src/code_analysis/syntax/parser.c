#include "minsc_test/code_analysis/syntax/parser.h"

#include <buf/buf.h>
#include <minsc/code_analysis/syntax/expression_syntax.h>
#include <minsc/code_analysis/syntax/syntax_facts.h>
#include <minsc/code_analysis/syntax/syntax_kind.h>
#include <minsc/code_analysis/syntax/syntax_node.h>
#include <minsc/code_analysis/syntax/syntax_tree.h>
#include <stddef.h>
#include <stdint.h>
#include <str/str.h>
#include <test/test.h>

#include "minsc_test/util/asserting_iterator.h"

static TEST_FUNC(state, binary_expression_honors_precedence, SyntaxKind op1, SyntaxKind op2) {
    size_t op1_precedence = binary_operator_precedence(op1);
    size_t op2_precedence = binary_operator_precedence(op2);
    str op1_text = syntax_facts_get_text(op1);
    str op2_text = syntax_facts_get_text(op2);
    str text = str_printf("a " str_fmt " b " str_fmt " c", str_arg(op1_text), str_arg(op2_text));

    SyntaxTree* tree = syntax_tree_parse(text);
    ExpressionSyntax* expression = tree->root->expression;

    AssertingIterator* it = asserting_iterator_new((const SyntaxNode*)expression);
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_node,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_BINARY_EXPRESSION
    );

    if (op1_precedence >= op2_precedence) {
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_BINARY_EXPRESSION
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_NAME_EXPRESSION
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_token,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_IDENTIFIER_TOKEN,
            str_lit("a")
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_token,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            op1,
            op1_text
        );
    } else {
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_NAME_EXPRESSION
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_token,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_IDENTIFIER_TOKEN,
            str_lit("a")
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_token,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            op1,
            op1_text
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_BINARY_EXPRESSION
        );
    }
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_node,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_NAME_EXPRESSION
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_token,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_IDENTIFIER_TOKEN,
        str_lit("b")
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_token,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        op2,
        op2_text
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_node,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_NAME_EXPRESSION
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_token,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_IDENTIFIER_TOKEN,
        str_lit("c")
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_end,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it
    );
    asserting_iterator_free(it);
    syntax_tree_free(tree);
    PASS();
}

static TEST_FUNC(
    state,
    unary_expression_honors_precedence,
    SyntaxKind unary_kind,
    SyntaxKind binary_kind
) {
    size_t unary_precedence = unary_operator_precedence(unary_kind);
    size_t binary_precedence = binary_operator_precedence(binary_kind);
    str unary_text = syntax_facts_get_text(unary_kind);
    str binary_text = syntax_facts_get_text(binary_kind);
    str text = str_printf(str_fmt "a " str_fmt " b", str_arg(unary_text), str_arg(binary_text));

    SyntaxTree* tree = syntax_tree_parse(text);
    ExpressionSyntax* expression = tree->root->expression;

    AssertingIterator* it = asserting_iterator_new((const SyntaxNode*)expression);
    if (unary_precedence >= binary_precedence) {
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_BINARY_EXPRESSION
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_UNARY_EXPRESSION
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_token,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            unary_kind,
            unary_text
        );
    } else {
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_UNARY_EXPRESSION
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_token,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            unary_kind,
            unary_text
        );
        RUN_SUBTEST(
            state,
            asserting_iterator_assert_node,
            CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
            it,
            SYNTAX_KIND_BINARY_EXPRESSION
        );
    }
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_node,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_NAME_EXPRESSION
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_token,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_IDENTIFIER_TOKEN,
        str_lit("a")
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_token,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        binary_kind,
        binary_text
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_node,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_NAME_EXPRESSION
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_token,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it,
        SYNTAX_KIND_IDENTIFIER_TOKEN,
        str_lit("b")
    );
    RUN_SUBTEST(
        state,
        asserting_iterator_assert_end,
        CLEANUP(asserting_iterator_free(it); syntax_tree_free(tree)),
        it
    );
    asserting_iterator_free(it);
    syntax_tree_free(tree);
    PASS();
}

SUITE_FUNC(state, parser) {
    SyntaxKindBuf binary_kinds = syntax_facts_get_binary_operator_kinds();
    for (uint64_t i = 0; i < binary_kinds.len; i++) {
        for (uint64_t j = 0; j < binary_kinds.len; j++) {
            RUN_TEST(
                state,
                binary_expression_honors_precedence,
                str_printf(
                    "binary expression honors precedence: " str_fmt ", " str_fmt,
                    str_arg(syntax_kind_string(binary_kinds.ptr[i])),
                    str_arg(syntax_kind_string(binary_kinds.ptr[j]))
                ),
                binary_kinds.ptr[i],
                binary_kinds.ptr[j]
            );
        }
    }

    SyntaxKindBuf unary_kinds = syntax_facts_get_unary_operator_kinds();
    for (uint64_t i = 0; i < unary_kinds.len; i++) {
        for (uint64_t j = 0; j < binary_kinds.len; j++) {
            RUN_TEST(
                state,
                unary_expression_honors_precedence,
                str_printf(
                    "unary expression honors precedence: " str_fmt ", " str_fmt,
                    str_arg(syntax_kind_string(unary_kinds.ptr[i])),
                    str_arg(syntax_kind_string(binary_kinds.ptr[j]))
                ),
                unary_kinds.ptr[i],
                binary_kinds.ptr[j]
            );
        }
    }
    BUF_FREE(unary_kinds);
    BUF_FREE(binary_kinds);
}
