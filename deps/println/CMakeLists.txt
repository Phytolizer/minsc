cmake_minimum_required(VERSION 3.23)

project(
  println
  LANGUAGES C
  DESCRIPTION "Minimal printfln() functions for C"
)

add_library(println src/println.c)
find_path(HEDLEY_INCLUDE_DIR hedley.h)
target_include_directories(println PUBLIC ${HEDLEY_INCLUDE_DIR} include)
add_library(println::println ALIAS println)
