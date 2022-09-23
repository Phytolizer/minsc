#ifndef TEST_TEST_H_
#define TEST_TEST_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <str/str.h>

#ifndef TEST_ABORT_ON_FAILURE
#define TEST_ABORT_ON_FAILURE 0
#endif

typedef struct {
    uint64_t passed;
    uint64_t failed;
    uint64_t skipped;
    uint64_t assertions;
    bool verbose;
} TestState;

typedef enum {
#define X(x) TEST_RESULT_##x,
#include "test_results.def"
#undef X
} TestResultType;

typedef struct {
    TestResultType type;
    str errorMessage;
} TestResult;

#define TEST_FAIL(msg) ((TestResult){.type = TEST_RESULT_FAIL, .errorMessage = msg})
#define TEST_PASS() ((TestResult){.type = TEST_RESULT_PASS})
#define TEST_SKIP() ((TestResult){.type = TEST_RESULT_SKIP})

#if TEST_ABORT_ON_FAILURE
#define TEST_ABORT() abort()
#else
#define TEST_ABORT()
#endif
#define TEST_ASSERT(state, test, cleanup, ...) \
    do { \
        ++(state)->assertions; \
        if (!(test)) { \
            str message_ = str_printf(__VA_ARGS__); \
            cleanup; \
            TEST_ABORT(); \
            return TEST_FAIL(message_); \
        } \
    } while (false)

#define FAIL(state, cleanup, ...) TEST_ASSERT(state, false, cleanup, __VA_ARGS__)

#define NO_CLEANUP (void)0
#define CLEANUP(x) x

#define RUN_SUITE(state, name, displayname) \
    do { \
        (void)fprintf(stderr, "SUITE " str_fmt "\n", str_arg(displayname)); \
        str_free(displayname); \
        name##_test_suite(state); \
    } while (false)

#define TEST_CALL(state, name, ...) name##_test(state, __VA_ARGS__)
#define TEST_CALL0(state, name) name##_test(state)

#define DO_RUN_TEST(state, name, call, displayname) \
    do { \
        str disp = displayname; \
        if ((state)->verbose) { \
            (void)fprintf(stderr, "TEST  " str_fmt "\n", str_arg(disp)); \
        } \
        TestResult result = call; \
        switch (result.type) { \
            case TEST_RESULT_FAIL: \
                ++(state)->failed; \
                (void)fprintf( \
                    stderr, \
                    "FAIL  " str_fmt ": " str_fmt "\n", \
                    str_arg(disp), \
                    str_arg(result.errorMessage) \
                ); \
                str_free(result.errorMessage); \
                break; \
            case TEST_RESULT_PASS: \
                ++(state)->passed; \
                break; \
            case TEST_RESULT_SKIP: \
                ++(state)->skipped; \
                (void)fprintf(stderr, "SKIP  " str_fmt "\n", str_arg(disp)); \
                break; \
        } \
        str_free(disp); \
    } while (false)

#define RUN_TEST(state, name, displayname, ...) \
    DO_RUN_TEST(state, name, TEST_CALL(state, name, __VA_ARGS__), displayname)

#define RUN_TEST0(state, name, displayname) \
    DO_RUN_TEST(state, name, TEST_CALL0(state, name), displayname)

#define DO_RUN_SUBTEST(state, name, call, cleanup) \
    do { \
        TestResult result = call; \
        if (result.type != TEST_RESULT_PASS) { \
            cleanup; \
            return result; \
        } \
    } while (false)

#define SUBTEST_CALL(state, name, ...) name##_subtest(state, __VA_ARGS__)
#define SUBTEST_CALL0(state, name) name##_subtest(state)

#define RUN_SUBTEST(state, name, cleanup, ...) \
    DO_RUN_SUBTEST(state, name, SUBTEST_CALL(state, name, __VA_ARGS__), cleanup)

#define RUN_SUBTEST0(state, name, cleanup) \
    DO_RUN_SUBTEST(state, name, SUBTEST_CALL0(state, name), cleanup)

#define SUITE_FUNC(state, name) void name##_test_suite(TestState* state)

#define TEST_FUNC(state, name, ...) TestResult name##_test(TestState* state, __VA_ARGS__)
#define TEST_FUNC0(state, name) TestResult name##_test(TestState* state)

#define SUBTEST_FUNC(state, name, ...) TestResult name##_subtest(TestState* state, __VA_ARGS__)
#define SUBTEST_FUNC0(state, name) TestResult name##_subtest(TestState* state)

#define PASS() return TEST_PASS()

#define SKIP() return TEST_SKIP()

#endif  // TEST_TEST_H_
