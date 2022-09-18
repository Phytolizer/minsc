cmake_minimum_required(VERSION 3.23)

project(hash LANGUAGES C)

add_library(hash src/hash.c)
target_include_directories(hash PUBLIC include)
target_link_libraries(hash PUBLIC str::str println::println)
add_library(hash::hash ALIAS hash)
