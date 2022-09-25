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

static inline TextSpan text_span_from_bounds(size_t start, size_t end) {
    return (TextSpan){start, end - start};
}

#endif  // MINSC_CODE_ANALYSIS_TEXT_SPAN_H_
