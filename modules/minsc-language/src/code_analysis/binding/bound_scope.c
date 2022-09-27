#include "minsc/code_analysis/binding/bound_scope.h"

#include <stdlib.h>
#include <uthash.h>

#include "minsc/support/minsc_assert.h"

typedef struct {
    str name;
    VariableSymbol variable;
    UT_hash_handle hh;
} ScopeMap;

typedef struct {
    ScopeMap* variables;
} BoundScopeImpl;

BoundScope* bound_scope_new(BoundScope* parent) {
    BoundScope* scope = malloc(sizeof(BoundScope));
    MINSC_ASSERT(scope != NULL);
    BoundScopeImpl* impl = malloc(sizeof(BoundScopeImpl));
    MINSC_ASSERT(impl != NULL);
    impl->variables = NULL;
    scope->impl = impl;
    scope->parent = parent;
    return scope;
}

void bound_scope_free(BoundScope* scope) {
    BoundScopeImpl* impl = scope->impl;
    ScopeMap* variable;
    ScopeMap* tmp;
    HASH_ITER(hh, impl->variables, variable, tmp) {
        HASH_DEL(impl->variables, variable);
        str_free(variable->name);
        variable_symbol_free(variable->variable);
        free(variable);
    }
    free(impl);
    free(scope);
}

static VariableSymbol* scope_map_find(ScopeMap* variables, str name) {
    ScopeMap* variable;
    // NOLINTNEXTLINE(readability-isolate-declaration): uthash needs this
    HASH_FIND(hh, variables, name.ptr, name.len, variable);
    return variable ? &variable->variable : NULL;
}

static void scope_map_insert(ScopeMap** variables, VariableSymbol variable) {
    ScopeMap* variable_entry = malloc(sizeof(ScopeMap));
    MINSC_ASSERT(variable_entry != NULL);
    variable_entry->name = str_ref(variable.name);
    variable_entry->variable = variable;
    // NOLINTNEXTLINE(readability-isolate-declaration): uthash needs this
    HASH_ADD_KEYPTR(
        hh,
        *variables,
        variable_entry->name.ptr,
        variable_entry->name.len,
        variable_entry
    );
}

bool bound_scope_try_declare(BoundScope* scope, VariableSymbol variable) {
    VariableSymbol* declared =
        scope_map_find(((BoundScopeImpl*)scope->impl)->variables, variable.name);
    if (declared != NULL) {
        return false;
    }

    scope_map_insert(&((BoundScopeImpl*)scope->impl)->variables, variable);
    return true;
}

BoundScopeTryLookupResult bound_scope_try_lookup(BoundScope* scope, str name) {
    for (BoundScope* current = scope; current != NULL; current = current->parent) {
        VariableSymbol* variable =
            scope_map_find(((BoundScopeImpl*)current->impl)->variables, name);
        if (variable != NULL) {
            return (BoundScopeTryLookupResult)SUM_JUST(*variable);
        }
    }

    return (BoundScopeTryLookupResult)SUM_NOTHING;
}

VariableSymbolBuf bound_scope_get_declared_variables(BoundScope* scope) {
    VariableSymbolBuf declared_variables = BUF_NEW;
    ScopeMap* variable;
    ScopeMap* tmp;
    HASH_ITER(hh, ((BoundScopeImpl*)scope->impl)->variables, variable, tmp) {
        BUF_PUSH(&declared_variables, variable->variable);
    }
    return declared_variables;
}
