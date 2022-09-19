#ifndef MINSC_RUNTIME_VARIABLE_MAP_H_
#define MINSC_RUNTIME_VARIABLE_MAP_H_

#include "minsc/code_analysis/variable_symbol.h"
#include "minsc/runtime/object.h"

typedef struct VariableMap VariableMap;

VariableMap* variable_map_new(void);
void variable_map_free(VariableMap* map);

void variable_map_define(VariableMap** map, VariableSymbol symbol, Object* value);
Object* variable_map_get(VariableMap* map, VariableSymbol symbol);

typedef bool (*VariableMapFindPredicate)(VariableSymbol* variable, void* user);

VariableSymbol* variable_map_find(VariableMap* map, VariableMapFindPredicate pred, void* user);

#endif  // MINSC_RUNTIME_VARIABLE_MAP_H_
