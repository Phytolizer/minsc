cmake_minimum_required(VERSION 3.23)

project(
  c-argparser
  LANGUAGES C
  HOMEPAGE_URL "https://github.com/Phytolizer/c-argparser"
)

add_library(argparser repo/str.c repo/arg.c)
target_include_directories(argparser PUBLIC repo/include)
add_library(argparser::argparser ALIAS argparser)
