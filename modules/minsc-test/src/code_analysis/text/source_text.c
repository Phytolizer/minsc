#include "minsc_test/code_analysis/text/source_text.h"

#include <buf/buf.h>
#include <minsc/code_analysis/text/source_text.h>
#include <stddef.h>
#include <stdint.h>
#include <str/str.h>
#include <test/test.h>

#include "minsc_test/util/str_esc.h"

typedef struct {
    str text;
    size_t expected_line_count;
} IncludesLastLineTest;

typedef BUF(const IncludesLastLineTest) IncludesLastLineTestBuf;

static IncludesLastLineTestBuf get_includes_last_line_data(void) {
    static const IncludesLastLineTest a[] = {
        {str_lit_c("."), 1},
        {str_lit_c(".\n"), 2},
        {str_lit_c(".\r\n"), 2},
        {str_lit_c(".\r"), 2},
        {str_lit_c(".\r\n\r\n"), 3},
        {str_lit_c(".\n\n"), 3},
    };

    return (IncludesLastLineTestBuf)BUF_ARRAY(a);
}

static TEST_FUNC(state, includes_last_line, IncludesLastLineTest test) {
    SourceText source = source_text_from(test.text);
    TEST_ASSERT(
        state,
        source.lines.len == test.expected_line_count,
        CLEANUP(source_text_free(source)),
        "Expected %zu lines, got %zu",
        test.expected_line_count,
        source.lines.len
    );
    source_text_free(source);
    PASS();
}

SUITE_FUNC(state, source_text) {
    IncludesLastLineTestBuf includes_last_line_data = get_includes_last_line_data();
    for (uint64_t i = 0; i < includes_last_line_data.len; i++) {
        IncludesLastLineTest test = includes_last_line_data.ptr[i];
        str text_esc = str_esc(test.text);
        RUN_TEST(
            state,
            includes_last_line,
            str_printf("includes last line ('" str_fmt "')", str_arg(text_esc)),
            test
        );
        str_free(text_esc);
    }
}
