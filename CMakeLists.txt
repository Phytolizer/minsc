cmake_minimum_required(VERSION 3.23)

project(minsc LANGUAGES C)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(PrependPath)
include(VaryCase)

set(CMAKE_C_STANDARD 11)

if(MINSC_SANITIZE)
  string(APPEND CMAKE_C_FLAGS
         " -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer"
  )
  string(APPEND CMAKE_EXE_LINKER_FLAGS
         " -fsanitize=address -fsanitize=undefined"
  )
endif()

add_subdirectory(deps/arena)
add_subdirectory(deps/buf)
add_subdirectory(deps/c-argparser)
add_subdirectory(deps/linenoise)
add_subdirectory(deps/println)
add_subdirectory(deps/str)
add_subdirectory(deps/styler)
add_subdirectory(deps/sum)
add_subdirectory(deps/varycase)

add_subdirectory(deps/file)
add_subdirectory(deps/uthash)

add_subdirectory(modules/minsc-language)
add_subdirectory(modules/minsc-compiler)
add_subdirectory(modules/minsc-test)

enable_testing()
add_test(NAME minsc::test COMMAND minsc::test)
