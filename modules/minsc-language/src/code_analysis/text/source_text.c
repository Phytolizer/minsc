#include "minsc/code_analysis/text/source_text.h"

static TextLineBuf parse_lines(SourceText* source, str text);
static size_t get_line_break_width(str text, size_t position);
static void add_line(
    TextLineBuf* lines,
    SourceText* source,
    size_t position,
    size_t line_start,
    size_t line_break_width
);

SourceText source_text_from(str text) {
    SourceText source;
    source.lines = parse_lines(&source, text);
    source.text = text;
    return source;
}

void source_text_free(SourceText source) {
    BUF_FREE(source.lines);
    str_free(source.text);
}

size_t source_text_get_line_index(SourceText source, size_t position) {
    size_t lower = 0;
    size_t upper = source.text.len - 1;

    while (lower <= upper) {
        size_t index = (lower + upper) / 2;
        TextLine line = source.lines.ptr[index];
        size_t start = line.start;

        if (position == start) {
            return index;
        }
        if (position < start) {
            upper = index - 1;
        } else {
            lower = index + 1;
        }
    }

    return lower - 1;
}

str source_text_substr(SourceText source, size_t start, size_t length) {
    return str_substr(source.text, start, length);
}

str source_text_substr_span(SourceText source, TextSpan span) {
    return source_text_substr(source, span.start, span.length);
}

static TextLineBuf parse_lines(SourceText* source, str text) {
    TextLineBuf result = BUF_NEW;

    size_t position = 0;
    size_t line_start = 0;

    while (position < text.len) {
        size_t line_break_width = get_line_break_width(text, position);

        if (line_break_width == 0) {
            position++;
        } else {
            add_line(&result, source, position, line_start, line_break_width);
            position += line_break_width;
            line_start = position;
        }
    }

    if (position >= line_start) {
        add_line(&result, source, position, line_start, 0);
    }

    return result;
}

static size_t get_line_break_width(str text, size_t position) {
    size_t result = 0;

    char c = text.ptr[position];
    // 'int' to suppress warning. char is promoted in the ternary.
    int l = position + 1 >= text.len ? '\0' : text.ptr[position + 1];

    if (c == '\r' && l == '\n') {
        result = 2;
    } else if (c == '\r' || c == '\n') {
        result = 1;
    }

    return result;
}

static void add_line(
    TextLineBuf* lines,
    SourceText* source,
    size_t position,
    size_t line_start,
    size_t line_break_width
) {
    size_t line_length = position - line_start;
    TextLine line = {
        .source = source,
        .start = line_start,
        .length = line_length,
        .length_including_line_break = line_length + line_break_width,
    };
    BUF_PUSH(lines, line);
}
