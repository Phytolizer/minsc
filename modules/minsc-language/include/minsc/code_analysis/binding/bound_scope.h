#ifndef MINSC_CODE_ANALYSIS_BINDING_BOUND_SCOPE_H_
#define MINSC_CODE_ANALYSIS_BINDING_BOUND_SCOPE_H_

#include <buf/buf.h>
#include <stdbool.h>
#include <sum/sum.h>

#include "minsc/code_analysis/variable_symbol.h"

typedef struct BoundScope {
    void* impl;
    struct BoundScope* parent;
} BoundScope;

BoundScope* bound_scope_new(BoundScope* parent);
void bound_scope_free(BoundScope* scope);
bool bound_scope_try_declare(BoundScope* scope, VariableSymbol variable);

typedef SUM_MAYBE_TYPE(VariableSymbol) BoundScopeTryLookupResult;

BoundScopeTryLookupResult bound_scope_try_lookup(BoundScope* scope, str name);

VariableSymbolBuf bound_scope_get_declared_variables(BoundScope* scope);

#endif  // MINSC_CODE_ANALYSIS_BINDING_BOUND_SCOPE_H_
