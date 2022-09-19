#ifndef MINSC_CODE_ANALYSIS_VARIABLE_SYMBOL_H_
#define MINSC_CODE_ANALYSIS_VARIABLE_SYMBOL_H_

#include <str/str.h>

#include "minsc/runtime/object.h"

typedef struct VariableSymbol {
    str name;
    ObjectType type;
} VariableSymbol;

VariableSymbol variable_symbol_new(str name, ObjectType type);
void variable_symbol_free(VariableSymbol symbol);

VariableSymbol variable_symbol_dup(VariableSymbol symbol);

#endif  // MINSC_CODE_ANALYSIS_VARIABLE_SYMBOL_H_
