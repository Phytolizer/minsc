add_library(styler src/styler.c)
target_include_directories(styler PUBLIC include)
add_library(styler::styler ALIAS styler)
