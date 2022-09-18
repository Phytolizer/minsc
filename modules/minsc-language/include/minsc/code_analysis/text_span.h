#ifndef MINSC_CODE_ANALYSIS_TEXT_SPAN_H_
#define MINSC_CODE_ANALYSIS_TEXT_SPAN_H_

#include <stddef.h>

typedef struct TextSpan {
    size_t start;
    size_t length;
} TextSpan;

static inline size_t text_span_end(TextSpan span) {
    return span.start + span.length;
}

#endif  // MINSC_CODE_ANALYSIS_TEXT_SPAN_H_
