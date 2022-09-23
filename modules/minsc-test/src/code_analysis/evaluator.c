#include "minsc_test/code_analysis/evaluator.h"

#include <buf/buf.h>
#include <minsc/code_analysis/compilation.h>
#include <minsc/code_analysis/syntax/syntax_tree.h>
#include <minsc/runtime/object.h>

typedef struct {
    str text;
    Object* value;
} EvaluatorTest;

typedef BUF(EvaluatorTest) EvaluatorTestBuf;

static void evaluator_test_buf_free(EvaluatorTestBuf buf) {
    for (size_t i = 0; i < buf.len; i++) {
        EvaluatorTest* test = &buf.ptr[i];
        str_free(test->text);
        object_free(test->value);
    }
    BUF_FREE(buf);
}

static EvaluatorTestBuf get_evaluator_tests(void) {
    EvaluatorTestBuf tests = BUF_NEW;
#define PUSH_TEST(t, v) \
    do { \
        EvaluatorTest test = { \
            .text = (t), \
            .value = (v), \
        }; \
        BUF_PUSH(&tests, test); \
    } while (0)
    PUSH_TEST(str_lit("1"), object_new_i64(1));
    PUSH_TEST(str_lit("+1"), object_new_i64(1));
    PUSH_TEST(str_lit("-1"), object_new_i64(-1));
    PUSH_TEST(str_lit("1 + 2"), object_new_i64(3));
    PUSH_TEST(str_lit("12 - 3"), object_new_i64(9));
    PUSH_TEST(str_lit("4 * 2"), object_new_i64(8));
    PUSH_TEST(str_lit("9 / 3"), object_new_i64(3));
    PUSH_TEST(str_lit("(10)"), object_new_i64(10));
    PUSH_TEST(str_lit("12 == 3"), object_new_bool(false));
    PUSH_TEST(str_lit("12 != 3"), object_new_bool(true));
    PUSH_TEST(str_lit("3 == 3"), object_new_bool(true));
    PUSH_TEST(str_lit("3 != 3"), object_new_bool(false));
    PUSH_TEST(str_lit("true"), object_new_bool(true));
    PUSH_TEST(str_lit("false"), object_new_bool(false));
    PUSH_TEST(str_lit("!true"), object_new_bool(false));
    PUSH_TEST(str_lit("!false"), object_new_bool(true));
    PUSH_TEST(str_lit("true == true"), object_new_bool(true));
    PUSH_TEST(str_lit("true == false"), object_new_bool(false));
    PUSH_TEST(str_lit("true != true"), object_new_bool(false));
    PUSH_TEST(str_lit("true != false"), object_new_bool(true));
    PUSH_TEST(str_lit("false == true"), object_new_bool(false));
    PUSH_TEST(str_lit("false == false"), object_new_bool(true));
    PUSH_TEST(str_lit("false != true"), object_new_bool(true));
    PUSH_TEST(str_lit("false != false"), object_new_bool(false));
    PUSH_TEST(str_lit("true && true"), object_new_bool(true));
    PUSH_TEST(str_lit("true && false"), object_new_bool(false));
    PUSH_TEST(str_lit("false && true"), object_new_bool(false));
    PUSH_TEST(str_lit("false && false"), object_new_bool(false));
    PUSH_TEST(str_lit("true || true"), object_new_bool(true));
    PUSH_TEST(str_lit("true || false"), object_new_bool(true));
    PUSH_TEST(str_lit("false || true"), object_new_bool(true));
    PUSH_TEST(str_lit("false || false"), object_new_bool(false));
    PUSH_TEST(str_lit("(a = 10) * a"), object_new_i64(100));
#undef PUSH_TEST
    return tests;
}

static str diagnostics_to_string(DiagnosticBuf diagnostics) {
    str result = str_null;
    for (size_t i = 0; i < diagnostics.len; i++) {
        Diagnostic diagnostic = diagnostics.ptr[i];
        str_append(&result, result, diagnostic.message, str_lit("\n"));
    }
    return result;
}

static TEST_FUNC(state, evaluator, EvaluatorTest test) {
    SyntaxTree* tree = syntax_tree_parse(str_ref(test.text));
    Compilation* compilation = compilation_new(tree);
    VariableMap* variables = variable_map_new();
    EvaluationResult result = compilation_evaluate(compilation, &variables);
    variable_map_free(variables);
    compilation_free(compilation);
    if (result.diagnostics != NULL) {
        str diagnostics_str = diagnostics_to_string(diagnostic_bag_iter(result.diagnostics));
        FAIL(
            state,
            CLEANUP(evaluation_result_free(result); str_free(diagnostics_str)),
            "Unexpected diagnostics:\n" str_fmt,
            str_arg(diagnostics_str)
        );
    }
    str expected_str = object_string(test.value);
    str actual_str = object_string(result.value);
    TEST_ASSERT(
        state,
        object_eq(result.value, test.value),
        CLEANUP(evaluation_result_free(result); str_free(expected_str); str_free(actual_str)),
        "Expected " str_fmt ", got " str_fmt,
        str_arg(expected_str),
        str_arg(actual_str)
    );
    str_free(expected_str);
    str_free(actual_str);
    evaluation_result_free(result);
    PASS();
}

SUITE_FUNC(state, evaluator) {
    EvaluatorTestBuf tests = get_evaluator_tests();

    for (uint64_t i = 0; i < tests.len; i++) {
        str obj_str = object_string(tests.ptr[i].value);
        RUN_TEST(
            state,
            evaluator,
            str_printf(str_fmt " => " str_fmt, str_arg(tests.ptr[i].text), str_arg(obj_str)),
            tests.ptr[i]
        );
        str_free(obj_str);
    }

    evaluator_test_buf_free(tests);
}
