cmake_minimum_required(VERSION 3.23)

project(minsc LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PrependPath)

set(CMAKE_C_STANDARD 11)

add_subdirectory(deps/buf)
add_subdirectory(deps/linenoise)
add_subdirectory(deps/println)
add_subdirectory(deps/str)
add_subdirectory(deps/styler)

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
)
prepend_path(MINSC_SRC_REL src/ MINSC_SRC)
add_executable(minsc ${MINSC_SRC})
target_link_libraries(
  minsc PRIVATE println::println linenoise::linenoise str::str buf::buf
                styler::styler
)
