cmake_minimum_required(VERSION 3.15)
project(jwc_embedded LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(gtu-code-mod STATIC
external/gtu-code-mod/parser.c
)

set_source_files_properties(external/gtu-code-mod/parser.c
  PROPERTIES LANGUAGE C
)

target_include_directories(gtu-code-mod PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/external/gtu-code-mod)

add_subdirectory(external/eigen)

add_subdirectory(apps)
