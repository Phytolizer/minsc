add_executable(
  minsc_test
  src/main.c
  src/code_analysis/evaluator.c
  src/code_analysis/syntax/lexer.c
  src/code_analysis/syntax/parser.c
  src/code_analysis/syntax/syntax_facts.c
  src/util/str_esc.c
  src/util/asserting_iterator.c
)

target_link_libraries(
  minsc_test PRIVATE minsc::language str::str println::println uthash::uthash
)
target_include_directories(minsc_test PRIVATE include)
add_executable(minsc::test ALIAS minsc_test)
