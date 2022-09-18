#include "minsc/code_analysis/compilation.h"

#include <buf/buf.h>
#include <stdlib.h>

#include "minsc/code_analysis/binding/binder.h"
#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/evaluator.h"
#include "minsc/code_analysis/syntax/diagnostic.h"
#include "minsc/runtime/object.h"
#include "minsc/support/minsc_assert.h"

struct Compilation {
    SyntaxTree* syntax;
};

Compilation* compilation_new(SyntaxTree* syntax) {
    Compilation* compilation = malloc(sizeof(Compilation));
    MINSC_ASSERT(compilation != NULL);
    compilation->syntax = syntax;
    return compilation;
}

void compilation_free(Compilation* compilation) {
    syntax_tree_free(compilation->syntax);
    free(compilation);
}

EvaluationResult compilation_evaluate(Compilation* compilation) {
    Binder* binder = binder_new();
    BoundExpression* bound_expression = binder_bind_expression(binder, compilation->syntax->root);

    DiagnosticBuf diagnostics = syntax_tree_take_diagnostics(compilation->syntax);
    DiagnosticBuf binder_diagnostics = binder_take_diagnostics(binder);
    BUF_CONCAT(&diagnostics, binder_diagnostics);
    BUF_FREE(binder_diagnostics);
    binder_free(binder);

    if (diagnostics.len > 0) {
        bound_expression_free(bound_expression);
        return (EvaluationResult){.diagnostics = diagnostics, .value = NULL};
    }

    Evaluator* evaluator = evaluator_new(bound_expression);
    Object* value = evaluator_evaluate(evaluator);
    bound_expression_free(bound_expression);
    evaluator_free(evaluator);

    return (EvaluationResult){.diagnostics = BUF_NEW, .value = value};
}
