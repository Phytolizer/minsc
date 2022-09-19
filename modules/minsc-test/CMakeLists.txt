add_executable(minsc-test src/main.c)

find_package(check REQUIRED)

target_link_libraries(minsc-test PRIVATE minsc::language Check::check str::str)
target_include_directories(minsc-test PRIVATE include)
add_executable(minsc::test ALIAS minsc-test)

enable_testing()
add_test(NAME minsc::test COMMAND minsc::test)
