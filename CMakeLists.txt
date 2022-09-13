cmake_minimum_required(VERSION 3.23)

project(minsc LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PrependPath)

add_subdirectory(deps/println)

set(MINSC_SRC main.c)
prepend_path(MINSC_SRC src/ MINSC_SRC_REL)
add_executable(minsc ${MINSC_SRC_REL})
target_link_libraries(minsc PRIVATE println::println)
