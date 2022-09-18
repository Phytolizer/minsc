#include "minsc/code_analysis/evaluation_result.h"

void evaluation_result_free(EvaluationResult result) {
    diagnostic_buf_free(result.diagnostics);
    object_free(result.value);
}
