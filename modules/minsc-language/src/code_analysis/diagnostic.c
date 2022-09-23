#include "minsc/code_analysis/diagnostic.h"

#include <str/str.h>

void diagnostic_free(Diagnostic diagnostic) {
    str_free(diagnostic.message);
}
