#include "diagnostic.h"

#include <buf/buf.h>
#include <stdint.h>
#include <str/str.h>

void diagnostic_buf_free(DiagnosticBuf diagnostics) {
    for (uint64_t i = 0; i < diagnostics.len; i++) {
        str_free(diagnostics.ptr[i]);
    }
    BUF_FREE(diagnostics);
}
