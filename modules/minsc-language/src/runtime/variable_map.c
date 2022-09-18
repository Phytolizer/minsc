#include "minsc/runtime/variable_map.h"

#include <uthash.h>

#include "minsc/support/minsc_assert.h"

struct VariableMap {
    str name;
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
        str_free(variable->name);
        object_free(variable->value);
        free(variable);
    }
}

void variable_map_define(VariableMap** map, str name, Object* value) {
    VariableMap* variable;
    // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
    HASH_FIND(hh, *map, name.ptr, name.len, variable);
    if (variable == NULL) {
        variable = malloc(sizeof(VariableMap));
        MINSC_ASSERT(variable != NULL);
        variable->name = name;
        // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
        HASH_ADD_KEYPTR(hh, *map, variable->name.ptr, variable->name.len, variable);
    } else {
        object_free(variable->value);
        str_free(name);
    }
    variable->value = value;
}

Object* variable_map_get(VariableMap* map, str name) {
    VariableMap* variable;
    // NOLINTNEXTLINE(readability-isolate-declaration): uthash requires this
    HASH_FIND(hh, map, name.ptr, name.len, variable);
    if (variable == NULL) {
        return NULL;
    }
    return variable->value;
}
