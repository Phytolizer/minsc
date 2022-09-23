#include "minsc_test/util/asserting_iterator.h"

#include <buf/buf.h>
#include <minsc/code_analysis/syntax/syntax_token.h>
#include <minsc/support/minsc_assert.h>

typedef BUF(const SyntaxNode*) Inner;

struct AssertingIterator {
    Inner inner;
    size_t pos;
};

static Inner flatten(const SyntaxNode* root);

AssertingIterator* asserting_iterator_new(const SyntaxNode* root) {
    AssertingIterator* it = malloc(sizeof(AssertingIterator));
    MINSC_ASSERT(it != NULL);
    it->inner = flatten(root);
    it->pos = 0;
    return it;
}

void asserting_iterator_free(AssertingIterator* iterator) {
    BUF_FREE(iterator->inner);
    free(iterator);
}

SUBTEST_FUNC(state, asserting_iterator_assert_end, AssertingIterator* it) {
    TEST_ASSERT(
        state,
        it->pos == it->inner.len,
        NO_CLEANUP,
        "Expected end of iterator, got " str_fmt,
        str_arg(syntax_kind_string(syntax_node_kind(it->inner.ptr[it->pos])))
    );
    PASS();
}

SUBTEST_FUNC(
    state,
    asserting_iterator_assert_token,
    AssertingIterator* it,
    SyntaxKind kind,
    str text
) {
    TEST_ASSERT(state, it->pos < it->inner.len, NO_CLEANUP, "expected more tokens");
    const SyntaxNode* node = it->inner.ptr[it->pos];
    TEST_ASSERT(
        state,
        node->type == SYNTAX_NODE_TYPE_TOKEN,
        NO_CLEANUP,
        "expected token, got " str_fmt,
        str_arg(syntax_kind_string(syntax_node_kind(node)))
    );
    const SyntaxToken* token = (const SyntaxToken*)node;
    TEST_ASSERT(
        state,
        token->kind == kind,
        NO_CLEANUP,
        "expected token kind " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(kind)),
        str_arg(syntax_kind_string(token->kind))
    );
    TEST_ASSERT(
        state,
        str_eq(token->text, text),
        NO_CLEANUP,
        "expected token text " str_fmt ", got " str_fmt,
        str_arg(text),
        str_arg(token->text)
    );
    it->pos++;
    PASS();
}

SUBTEST_FUNC(state, asserting_iterator_assert_node, AssertingIterator* it, SyntaxKind kind) {
    TEST_ASSERT(state, it->pos < it->inner.len, NO_CLEANUP, "expected more tokens");
    const SyntaxNode* node = it->inner.ptr[it->pos];
    TEST_ASSERT(
        state,
        node->type != SYNTAX_NODE_TYPE_TOKEN,
        NO_CLEANUP,
        "expected " str_fmt ", got token '" str_fmt "'",
        str_arg(syntax_kind_string(kind)),
        str_arg(((const SyntaxToken*)node)->text)
    );
    TEST_ASSERT(
        state,
        syntax_node_kind(node) == kind,
        NO_CLEANUP,
        "expected " str_fmt ", got " str_fmt,
        str_arg(syntax_kind_string(kind)),
        str_arg(syntax_kind_string(syntax_node_kind(node)))
    );
    it->pos++;
    PASS();
}

static Inner flatten(const SyntaxNode* root) {
    Inner stack = BUF_NEW;
    BUF_PUSH(&stack, root);

    Inner result = BUF_NEW;

    while (stack.len > 0) {
        const SyntaxNode* node = BUF_POP(&stack);
        BUF_PUSH(&result, node);

        SyntaxNodeChildren children = syntax_node_children(node);
        for (size_t i = children.len; i > 0; i--) {
            BUF_PUSH(&stack, children.ptr[i - 1]);
        }
        BUF_FREE(children);
    }

    BUF_FREE(stack);
    return result;
}
