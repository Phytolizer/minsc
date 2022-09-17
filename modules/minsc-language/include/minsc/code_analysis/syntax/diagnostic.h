#ifndef MINSC_CODE_ANALYSIS_SYNTAX_DIAGNOSTIC_H_
#define MINSC_CODE_ANALYSIS_SYNTAX_DIAGNOSTIC_H_

#include <buf/buf.h>
#include <str/str.h>

typedef BUF(str) DiagnosticBuf;

extern void diagnostic_buf_free(DiagnosticBuf diagnostics);

#endif  // MINSC_CODE_ANALYSIS_SYNTAX_DIAGNOSTIC_H_
