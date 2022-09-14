#include "evaluator.h"

#include "binary_expression_syntax.h"
#include "literal_expression_syntax.h"
#include "minsc_assert.h"
#include "println/println.h"

struct Evaluator {
    ExpressionSyntax* root;
};

static int64_t evaluate_expression(ExpressionSyntax* expression);

Evaluator* evaluator_new(ExpressionSyntax* root) {
    Evaluator* evaluator = malloc(sizeof(Evaluator));
    MINSC_ASSERT(evaluator != NULL);
    evaluator->root = root;
    return evaluator;
}

void evaluator_free(Evaluator* evaluator) {
    free(evaluator);
}

int64_t evaluator_evaluate(Evaluator* evaluator) {
    return evaluate_expression(evaluator->root);
}

static int64_t evaluate_expression(ExpressionSyntax* expression) {
    switch (expression->type) {
        case EXPRESSION_SYNTAX_TYPE_LITERAL:
            return object_as_i64(((LiteralExpressionSyntax*)expression)
                                         ->literal_token->value);
        case EXPRESSION_SYNTAX_TYPE_BINARY: {
            BinaryExpressionSyntax* binary =
                    (BinaryExpressionSyntax*)expression;
            int64_t left = evaluate_expression(binary->left);
            int64_t right = evaluate_expression(binary->right);
            switch (binary->operator_token->kind) {
                case SYNTAX_KIND_PLUS_TOKEN:
                    return left + right;
                case SYNTAX_KIND_MINUS_TOKEN:
                    return left - right;
                case SYNTAX_KIND_STAR_TOKEN:
                    return left * right;
                case SYNTAX_KIND_SLASH_TOKEN:
                    return left / right;
                default:
                    MINSC_ASSERT(0);
            }
        }
    }

    fprintfln(stderr, "ERROR: corrupt expression or not handled in switch");
    abort();
}
