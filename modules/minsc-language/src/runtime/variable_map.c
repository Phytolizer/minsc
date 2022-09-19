#include "minsc/runtime/variable_map.h"

#include <uthash.h>

#include "minsc/code_analysis/variable_symbol.h"
#include "minsc/support/minsc_assert.h"

struct VariableMap {
    VariableSymbol symbol;
    Object* value;
    UT_hash_handle hh;
};

VariableMap* variable_map_new(void) {
    return NULL;
}

void variable_map_free(VariableMap* map) {
    VariableMap* variable;
    VariableMap* tmp;

    HASH_ITER(hh, map, variable, tmp) {
        HASH_DEL(map, variable);
        variable_symbol_free(variable->symbol);
        object_free(variable->value);
        free(variable);
    }
}

void variable_map_define(VariableMap** map, VariableSymbol symbol, Object* value) {
    VariableMap* variable;
    // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
    HASH_FIND(hh, *map, symbol.name.ptr, symbol.name.len, variable);
    if (variable == NULL) {
        variable = malloc(sizeof(VariableMap));
        MINSC_ASSERT(variable != NULL);
        variable->symbol = symbol;
        // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
        HASH_ADD_KEYPTR(hh, *map, variable->symbol.name.ptr, variable->symbol.name.len, variable);
    } else {
        object_free(variable->value);
        variable_symbol_free(symbol);
    }
    variable->value = value;
}

Object* variable_map_get(VariableMap* map, VariableSymbol symbol) {
    VariableMap* variable;
    // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
    HASH_FIND(hh, map, symbol.name.ptr, symbol.name.len, variable);
    if (variable == NULL) {
        return NULL;
    }
    return variable->value;
}

VariableSymbol* variable_map_find(VariableMap* map, VariableMapFindPredicate pred, void* user) {
    VariableMap* variable;
    VariableMap* tmp;

    HASH_ITER(hh, map, variable, tmp) {
        if (pred(&variable->symbol, user)) {
            return &variable->symbol;
        }
    }

    return NULL;
}
