#ifndef MINSC_TEST_UTIL_ASSERTING_ITERATOR_H_
#define MINSC_TEST_UTIL_ASSERTING_ITERATOR_H_

#include <minsc/code_analysis/syntax/syntax_kind.h>
#include <minsc/code_analysis/syntax/syntax_node.h>
#include <str/str.h>
#include <test/test.h>

typedef struct AssertingIterator AssertingIterator;

AssertingIterator* asserting_iterator_new(const SyntaxNode* root);
void asserting_iterator_free(AssertingIterator* iterator);

SUBTEST_FUNC(state, asserting_iterator_assert_end, AssertingIterator* it);
SUBTEST_FUNC(
    state,
    asserting_iterator_assert_token,
    AssertingIterator* it,
    SyntaxKind kind,
    str text
);
SUBTEST_FUNC(state, asserting_iterator_assert_node, AssertingIterator* it, SyntaxKind kind);

#endif  // MINSC_TEST_UTIL_ASSERTING_ITERATOR_H_
