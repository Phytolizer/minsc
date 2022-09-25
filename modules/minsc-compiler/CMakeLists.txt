add_executable(minsc_compiler src/main.c src/string_builder.c)
target_link_libraries(
  minsc_compiler PRIVATE minsc::language buf::buf linenoise::linenoise
                         println::println str::str styler::styler
)
add_executable(minsc::compiler ALIAS minsc_compiler)
