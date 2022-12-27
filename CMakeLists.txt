cmake_minimum_required(VERSION 3.20)

project(
  mcc
  DESCRIPTION "mcc - micro ansi-c compiler"
  LANGUAGES CXX
)

project(
  mcc-cmd
  DESCRIPTION "mcc command line interface"
  LANGUAGES CXX
)

project(
  mcc-test
  DESCRIPTION "mcc test suite"
  LANGUAGES CXX
)

include(cmake/dependencies.cmake)
include(cmake/project.cmake)

add_subdirectory(src/mcc)
add_subdirectory(src/cmd)
add_subdirectory(src/test)
