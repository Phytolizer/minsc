add_executable(
  minsc_test src/main.c src/code_analysis/syntax/lexer.c src/util/str_esc.c
)

find_package(check REQUIRED)

target_link_libraries(
  minsc_test PRIVATE minsc::language Check::check str::str println::println
)
target_include_directories(minsc_test PRIVATE include)
add_executable(minsc::test ALIAS minsc_test)

enable_testing()
add_test(NAME minsc::test COMMAND minsc::test)
