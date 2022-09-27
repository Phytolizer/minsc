#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_GLOBAL_SCOPE_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_GLOBAL_SCOPE_H_

#include <buf/buf.h>

#include "minsc/code_analysis/binding/bound_expression.h"
#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/code_analysis/variable_symbol.h"

typedef struct BoundGlobalScope {
    struct BoundGlobalScope* previous;
    DiagnosticBuf diagnostics;
    VariableSymbolBuf variables;
    BoundExpression* expression;
} BoundGlobalScope;

BoundGlobalScope* bound_global_scope_new(
    BoundGlobalScope* previous,
    DiagnosticBuf diagnostics,
    VariableSymbolBuf variables,
    BoundExpression* expression
);
void bound_global_scope_free(BoundGlobalScope* scope);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_GLOBAL_SCOPE_H_
