cmake_minimum_required(VERSION 3.23)

project(minsc LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PrependPath)
include(VaryCase)

set(CMAKE_C_STANDARD 11)

add_subdirectory(deps/buf)
add_subdirectory(deps/c-argparser)
add_subdirectory(deps/linenoise)
add_subdirectory(deps/println)
add_subdirectory(deps/str)
add_subdirectory(deps/styler)
add_subdirectory(deps/sum)
add_subdirectory(deps/varycase)

add_subdirectory(deps/file)

set(MINSC_SRC_REL
    main.c
    code_analysis/binding/binder.c
    code_analysis/binding/bound_binary_expression.c
    code_analysis/binding/bound_expression.c
    code_analysis/binding/bound_literal_expression.c
    code_analysis/binding/bound_node.c
    code_analysis/binding/bound_unary_expression.c
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
add_executable(minsc ${MINSC_SRC})
set(MINSC_INC code_analysis/syntax/syntax_kind.inc runtime/object_type.inc)
foreach(inc ${MINSC_INC})
  cmake_path(GET inc PARENT_PATH inc_dir)
  cmake_path(
    ABSOLUTE_PATH inc BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/minsc
  )
  vary_case(${inc} include/minsc/${inc_dir} uppercamel minsc)
endforeach()
target_include_directories(minsc PRIVATE include)
target_link_libraries(
  minsc PRIVATE println::println linenoise::linenoise str::str buf::buf
                styler::styler sum::sum
)
