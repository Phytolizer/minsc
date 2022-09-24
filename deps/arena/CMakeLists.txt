cmake_minimum_required(VERSION 3.23)

project(arena LANGUAGES C)

set(ARENA_BACKEND
    "LIBC_MALLOC"
    CACHE STRING "The backend to use for the arena allocator"
)

set(ARENA_BACKENDS "LIBC_MALLOC;LINUX_MMAP;WIN32_VIRTUALALLOC;WASM_HEAPBASE")

if(NOT ARENA_BACKEND IN_LIST ARENA_BACKENDS)
  message(FATAL_ERROR "Invalid ARENA_BACKEND: ${ARENA_BACKEND}")
endif()

add_library(arena src/arena.c)
target_compile_definitions(arena PRIVATE ARENA_BACKEND_${ARENA_BACKEND})
target_include_directories(arena PUBLIC include)
