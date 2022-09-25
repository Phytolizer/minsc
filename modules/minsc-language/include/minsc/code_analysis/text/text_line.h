#ifndef MINSC_CODE_ANALYSIS_TEXT_TEXT_LINE_H_
#define MINSC_CODE_ANALYSIS_TEXT_TEXT_LINE_H_

#include <stddef.h>
#include <str/str.h>

#include "minsc/code_analysis/text/text_span.h"

struct SourceText;

typedef struct TextLine {
    struct SourceText* source;
    size_t start;
    size_t length;
    size_t length_including_line_break;
} TextLine;

static inline TextSpan text_line_span(TextLine line) {
    return (TextSpan){
        .start = line.start,
        .length = line.length,
    };
}

static inline TextSpan text_line_span_including_line_break(TextLine line) {
    return (TextSpan){
        .start = line.start,
        .length = line.length_including_line_break,
    };
}

static inline size_t text_line_end(TextLine line) {
    return line.start + line.length;
}

str text_line_to_string(TextLine line);

#endif  // MINSC_CODE_ANALYSIS_TEXT_TEXT_LINE_H_
