#ifndef MINSC_CODE_ANALYSIS_COMPILATION_H_
#define MINSC_CODE_ANALYSIS_COMPILATION_H_

#include "minsc/code_analysis/evaluation_result.h"
#include "minsc/code_analysis/syntax/syntax_tree.h"
#include "minsc/runtime/variable_map.h"

typedef struct Compilation Compilation;

Compilation* compilation_new(SyntaxTree* syntax);
void compilation_free(Compilation* compilation);

EvaluationResult compilation_evaluate(const Compilation* compilation, VariableMap** variables);

#endif  // MINSC_CODE_ANALYSIS_COMPILATION_H_
