#include "minsc/code_analysis/variable_symbol.h"

#include <str/str.h>

VariableSymbol variable_symbol_new(str name, ObjectType type) {
    return (VariableSymbol){.name = name, .type = type};
}

void variable_symbol_free(VariableSymbol symbol) {
    str_free(symbol.name);
}

VariableSymbol variable_symbol_dup(VariableSymbol symbol) {
    return variable_symbol_new(str_dup(symbol.name), symbol.type);
}
