cmake_minimum_required(VERSION 3.23)

project(
  varycase
  LANGUAGES C
  DESCRIPTION "Convert X-macro definitions to have different case styles"
)

configure_file(cmake/config.h.cmake-in include/config.h)

add_executable(varycase src/varycase.c)
target_link_libraries(
  varycase PRIVATE argparser::argparser println::println str::str sum::sum
                   file::file
)
target_include_directories(
  varycase PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include"
)
add_executable(varycase::varycase ALIAS varycase)
