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

EvaluationResult compilation_evaluate(const Compilation* compilation, VariableMap** variables) {
    BoundGlobalScope* global_scope = binder_bind_global_scope(compilation->syntax->root);
    BoundExpression* bound_expression = global_scope->expression;
    global_scope->expression = NULL;

    DiagnosticBag* diagnostics = syntax_tree_take_diagnostics(compilation->syntax);
    diagnostic_bag_concat(diagnostics, &global_scope->diagnostics);
    bound_global_scope_free(global_scope);

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
