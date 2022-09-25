#include "minsc/code_analysis/text/text_line.h"

#include "minsc/code_analysis/text/source_text.h"

str text_line_to_string(TextLine line) {
    return source_text_substr(*line.source, line.start, line.length);
}
