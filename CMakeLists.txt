cmake_minimum_required(VERSION 3.23)

project(minsc LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PrependPath)

add_subdirectory(deps/linenoise)
add_subdirectory(deps/println)
add_subdirectory(deps/str)

set(MINSC_SRC_REL main.c lexer.c object.c syntax_kind.c syntax_token.c)
prepend_path(MINSC_SRC_REL src/ MINSC_SRC)
add_executable(minsc ${MINSC_SRC})
target_link_libraries(
  minsc PRIVATE println::println linenoise::linenoise str::str
)
