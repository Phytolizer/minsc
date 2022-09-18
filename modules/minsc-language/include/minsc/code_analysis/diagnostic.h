#ifndef MINSC_CODE_ANALYSIS_DIAGNOSTIC_H_
#define MINSC_CODE_ANALYSIS_DIAGNOSTIC_H_

#include <buf/buf.h>
#include <str/str.h>

#include "minsc/code_analysis/text_span.h"

typedef struct Diagnostic {
    TextSpan span;
    str message;
} Diagnostic;

void diagnostic_free(Diagnostic diagnostic);

#endif  // MINSC_CODE_ANALYSIS_DIAGNOSTIC_H_
