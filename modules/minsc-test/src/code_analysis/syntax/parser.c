#include "minsc_test/code_analysis/syntax/parser.h"

#include <minsc/code_analysis/syntax/syntax_facts.h>
#include <minsc/code_analysis/syntax/syntax_kind.h>
#include <minsc/code_analysis/syntax/syntax_tree.h>

#include "minsc_test/util/asserting_iterator.h"

static TEST_FUNC(state, binary_expression_honors_precedence, SyntaxKind op1, SyntaxKind op2) {
    size_t op1_precedence = binary_operator_precedence(op1);
    size_t op2_precedence = binary_operator_precedence(op2);
    str op1_text = syntax_facts_get_text(op1);
    str op2_text = syntax_facts_get_text(op2);
    str text = str_printf("a " str_fmt " b " str_fmt " c", str_arg(op1_text), str_arg(op2_text));

    SyntaxTree* tree = syntax_tree_parse(text);
    ExpressionSyntax* expression = tree->root;

    if (op1_precedence >= op2_precedence) {
        AssertingIterator* it = asserting_iterator_new((const SyntaxNode*)expression);
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
    } else {
        AssertingIterator* it = asserting_iterator_new((const SyntaxNode*)expression);
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
    }
    syntax_tree_free(tree);
    PASS();
}

typedef struct {
    SyntaxKind op1;
    SyntaxKind op2;
} BinaryOperatorPair;

SUITE_FUNC(state, parser) {
    SyntaxKindBuf kinds = syntax_facts_get_binary_operator_kinds();
    for (uint64_t i = 0; i < kinds.len; i++) {
        for (uint64_t j = 0; j < kinds.len; j++) {
            RUN_TEST(
                state,
                binary_expression_honors_precedence,
                str_printf(
                    "binary expression honors precedence: " str_fmt ", " str_fmt,
                    str_arg(syntax_kind_string(kinds.ptr[i])),
                    str_arg(syntax_kind_string(kinds.ptr[j]))
                ),
                kinds.ptr[i],
                kinds.ptr[j]
            );
        }
    }
    BUF_FREE(kinds);
}
