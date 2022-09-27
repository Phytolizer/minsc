#include "minsc/code_analysis/binding/bound_global_scope.h"

#include <stdlib.h>

#include "minsc/support/minsc_assert.h"

BoundGlobalScope* bound_global_scope_new(
    BoundGlobalScope* previous,
    DiagnosticBuf diagnostics,
    VariableSymbolBuf variables,
    BoundExpression* expression
) {
    BoundGlobalScope* scope = malloc(sizeof(BoundGlobalScope));
    MINSC_ASSERT(scope != NULL);
    scope->previous = previous;
    scope->diagnostics = diagnostics;
    scope->variables = variables;
    scope->expression = expression;
    return scope;
}

void bound_global_scope_free(BoundGlobalScope* scope) {
    BUF_FREE(scope->diagnostics);
    BUF_FREE(scope->variables);
    bound_expression_free(scope->expression);
    free(scope);
}
