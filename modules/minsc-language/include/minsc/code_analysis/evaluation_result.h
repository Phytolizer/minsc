#ifndef MINSC_CODE_ANALYSIS_EVALUATION_RESULT_H_
#define MINSC_CODE_ANALYSIS_EVALUATION_RESULT_H_

#include "minsc/code_analysis/diagnostic_bag.h"
#include "minsc/runtime/object.h"

typedef struct EvaluationResult {
    DiagnosticBag* diagnostics;
    Object* value;
} EvaluationResult;

void evaluation_result_free(EvaluationResult result);

#endif  // MINSC_CODE_ANALYSIS_EVALUATION_RESULT_H_
