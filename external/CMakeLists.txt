cmake_minimum_required(VERSION 3.15)
project(jwc_embedded)

set(GTU_SOURCES
  "gtu-code-mod/calc.c"
  "gtu-code-mod/parser.c"
  "gtu-code-mod/debug.c"
)
add_library(gtu_lib STATIC ${GTU_SOURCES})
target_include_directories(gtu_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gtu-code)

add_subdirectory(eigen EXCLUDE_FROM_ALL)
