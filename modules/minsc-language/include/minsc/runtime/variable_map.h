#ifndef MINSC_RUNTIME_VARIABLE_MAP_H_
#define MINSC_RUNTIME_VARIABLE_MAP_H_

#include "minsc/runtime/object.h"

typedef struct VariableMap VariableMap;

VariableMap* variable_map_new(void);
void variable_map_free(VariableMap* map);

void variable_map_define(VariableMap** map, str name, Object* value);
Object* variable_map_get(VariableMap* map, str name);

#endif  // MINSC_RUNTIME_VARIABLE_MAP_H_
