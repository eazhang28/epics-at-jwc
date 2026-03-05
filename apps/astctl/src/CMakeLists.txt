cmake_minimum_required(VERSION 3.15)
project(jwc_embedded)

add_executable(jwc_embedded
  main.cpp
  tokenizer.cpp
  driver.cpp
  sqlut.cpp
  sequencer.cpp
)

target_include_directories(jwc_embedded PRIVATE
  ${CMAKE_SOURCE_DIR}/external/gtu-code-mod
  ${CMAKE_SOURCE_DIR}/apps/astctl/src
)

target_compile_options(jwc_embedded PRIVATE -fpermissive)

target_link_libraries(jwc_embedded PRIVATE
  gtu-code-mod
  eigen
  sqlite3
)

