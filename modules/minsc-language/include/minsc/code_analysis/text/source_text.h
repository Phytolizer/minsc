#ifndef MINSC_CODE_ANALYSIS_TEXT_SOURCE_TEXT_H_
#define MINSC_CODE_ANALYSIS_TEXT_SOURCE_TEXT_H_

#include <buf/buf.h>
#include <str/str.h>

#include "minsc/code_analysis/text/text_line.h"

typedef BUF(TextLine) TextLineBuf;

typedef struct SourceText {
    TextLineBuf lines;
    str text;
} SourceText;

SourceText source_text_from(str text);
void source_text_free(SourceText source);
size_t source_text_get_line_index(SourceText source, size_t position);
str source_text_substr(SourceText source, size_t start, size_t length);
str source_text_substr_span(SourceText source, TextSpan span);

#endif  // MINSC_CODE_ANALYSIS_TEXT_SOURCE_TEXT_H_
