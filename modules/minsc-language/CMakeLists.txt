set(MINSC_SRC_REL
    code_analysis/binding/binder.c
    code_analysis/binding/bound_binary_expression.c
    code_analysis/binding/bound_binary_operator.c
    code_analysis/binding/bound_expression.c
    code_analysis/binding/bound_literal_expression.c
    code_analysis/binding/bound_node.c
    code_analysis/binding/bound_unary_expression.c
    code_analysis/binding/bound_unary_operator.c
    code_analysis/compilation.c
    code_analysis/evaluation_result.c
    code_analysis/evaluator.c
    code_analysis/syntax/lexer.c
    code_analysis/syntax/syntax_kind.c
    code_analysis/syntax/syntax_token.c
    code_analysis/syntax/parser.c
    code_analysis/syntax/binary_expression_syntax.c
    code_analysis/syntax/expression_syntax.c
    code_analysis/syntax/literal_expression_syntax.c
    code_analysis/syntax/syntax_node.c
    code_analysis/syntax/diagnostic.c
    code_analysis/syntax/syntax_tree.c
    code_analysis/syntax/parenthesized_expression_syntax.c
    code_analysis/syntax/syntax_facts.c
    code_analysis/syntax/unary_expression_syntax.c
    runtime/object.c
)
prepend_path(MINSC_SRC_REL src/ MINSC_SRC)
add_library(minsc_language ${MINSC_SRC})
target_include_directories(minsc_language PUBLIC include)
set(MINSC_INC code_analysis/syntax/syntax_kind.inc runtime/object_type.inc)
foreach(inc ${MINSC_INC})
  cmake_path(GET inc PARENT_PATH inc_dir)
  cmake_path(
    ABSOLUTE_PATH inc BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/minsc
  )
  vary_case(${inc} include/minsc/${inc_dir} uppercamel minsc_language)
endforeach()
target_link_libraries(
  minsc_language PRIVATE println::println linenoise::linenoise str::str
                         buf::buf styler::styler sum::sum
)
add_library(minsc::language ALIAS minsc_language)
