#include "minsc/code_analysis/diagnostic.h"

#include <buf/buf.h>
#include <stdint.h>
#include <str/str.h>

void diagnostic_free(Diagnostic diagnostic) {
    str_free(diagnostic.message);
}
