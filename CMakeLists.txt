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
    code_analysis/syntax/evaluator.c
    code_analysis/syntax/parenthesized_expression_syntax.c
    code_analysis/syntax/syntax_facts.c
    code_analysis/syntax/unary_expression_syntax.c
    runtime/object.c
)
prepend_path(MINSC_SRC_REL src/ MINSC_SRC)
add_executable(minsc ${MINSC_SRC})
set(MINSC_INC_REL syntax_kind.inc)
prepend_path(MINSC_INC_REL include/minsc/code_analysis/syntax MINSC_INC)
foreach(inc ${MINSC_INC})
  message(STATUS "${inc}")
  vary_case(${inc} include/minsc/code_analysis/syntax uppercamel minsc)
endforeach()
target_include_directories(minsc PRIVATE include)
target_link_libraries(
  minsc PRIVATE println::println linenoise::linenoise str::str buf::buf
                styler::styler
)
