#include <inttypes.h>
#include <println/println.h>
#include <test/test.h>

#include "minsc_test/code_analysis/syntax/lexer.h"
#include "minsc_test/code_analysis/syntax/parser.h"
#include "minsc_test/code_analysis/syntax/syntax_facts.h"

int main(void) {
    TestState state = {0};
    RUN_SUITE(&state, lexer, str_lit("Lexer"));
    RUN_SUITE(&state, parser, str_lit("Parser"));
    RUN_SUITE(&state, syntax_facts, str_lit("Syntax Facts"));

    // print test summary
    printfln(
        "tests: %" PRIu64 ", passed: %" PRIu64 ", failed: %" PRIu64 ", skipped: %" PRIu64
        ", assertions: %" PRIu64,
        state.passed + state.failed,
        state.passed,
        state.failed,
        state.skipped,
        state.assertions
    );
    return (int)state.failed;
}
