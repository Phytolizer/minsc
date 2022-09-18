#include "minsc/runtime/variable_map.h"

VariableMap variable_map_new(void) {
    return (VariableMap)HASH_STRING_HASH_NEW;
}

void variable_map_free(VariableMap map) {
    HASH_STRING_HASH_FREE(map, object_free);
}

void variable_map_define(VariableMap* map, str name, Object* value) {
    HASH_STRING_HASH_INSERT(VariableMap, map, name, value);
}

Object* variable_map_get(VariableMap* map, str name) {
    Object* value;
    HASH_STRING_HASH_FIND(VariableMap, map, name, &value);
    return value;
}
