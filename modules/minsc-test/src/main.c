#include <inttypes.h>
#include <println/println.h>
#include <test/test.h>

#include "minsc_test/code_analysis/syntax/lexer.h"

int main(void) {
    TestState state = {0};
    RUN_SUITE(&state, lexer, str_lit("Lexer"));

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
