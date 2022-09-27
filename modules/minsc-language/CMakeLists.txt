set(MINSC_SRC_REL
    code_analysis/binding/binder.c
    code_analysis/binding/bound_variable_expression.c
    code_analysis/binding/bound_assignment_expression.c
    code_analysis/binding/bound_binary_expression.c
    code_analysis/binding/bound_binary_operator.c
    code_analysis/binding/bound_expression.c
    code_analysis/binding/bound_literal_expression.c
    code_analysis/binding/bound_node.c
    code_analysis/binding/bound_unary_expression.c
    code_analysis/binding/bound_unary_operator.c
    code_analysis/binding/bound_scope.c
    code_analysis/binding/bound_global_scope.c
    code_analysis/compilation.c
    code_analysis/diagnostic.c
    code_analysis/diagnostic_bag.c
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
    code_analysis/syntax/syntax_tree.c
    code_analysis/syntax/parenthesized_expression_syntax.c
    code_analysis/syntax/syntax_facts.c
    code_analysis/syntax/unary_expression_syntax.c
    code_analysis/syntax/name_expression_syntax.c
    code_analysis/syntax/assignment_expression_syntax.c
    code_analysis/syntax/compilation_unit_syntax.c
    code_analysis/text/source_text.c
    code_analysis/text/text_line.c
    code_analysis/variable_symbol.c
    runtime/object.c
    runtime/variable_map.c
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
  minsc_language
  PRIVATE println::println linenoise::linenoise styler::styler sum::sum
          uthash::uthash
  PUBLIC str::str buf::buf
)
add_library(minsc::language ALIAS minsc_language)
