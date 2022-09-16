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
    lexer.c
    object.c
    syntax_kind.c
    syntax_token.c
    parser.c
    binary_expression_syntax.c
    expression_syntax.c
    literal_expression_syntax.c
    syntax_node.c
    diagnostic.c
    syntax_tree.c
    evaluator.c
    parenthesized_expression_syntax.c
    syntax_facts.c
    unary_expression_syntax.c
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
