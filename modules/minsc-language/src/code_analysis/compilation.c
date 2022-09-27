#include "minsc/code_analysis/compilation.h"

#include <stdlib.h>

#include "minsc/code_analysis/binding/binder.h"
#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/code_analysis/evaluator.h"
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
    free(compilation);
}

EvaluationResult compilation_evaluate(Compilation* compilation, VariableMap** variables) {
    Binder* binder = binder_new(variables);
    BoundExpression* bound_expression = binder_bind_expression(binder, compilation->syntax->root->expression);

    DiagnosticBag* diagnostics = syntax_tree_take_diagnostics(compilation->syntax);
    diagnostic_bag_concat(diagnostics, binder_take_diagnostics(binder));
    binder_free(binder);

    if (!diagnostic_bag_empty(diagnostics)) {
        bound_expression_free(bound_expression);
        return (EvaluationResult){.diagnostics = diagnostics, .value = NULL};
    }

    diagnostic_bag_free(diagnostics);
    Evaluator* evaluator = evaluator_new(bound_expression, variables);
    Object* value = evaluator_evaluate(evaluator);
    bound_expression_free(bound_expression);
    evaluator_free(evaluator);

    return (EvaluationResult){.diagnostics = NULL, .value = value};
}
