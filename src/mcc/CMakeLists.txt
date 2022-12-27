file(
  GLOB_RECURSE MCC_SOURCE
  ${MCC_FILE_REGEX} *.hpp
  ${MCC_FILE_REGEX} *.cpp
)

add_library(
  mcc STATIC
  ${MCC_SOURCE}
)

target_include_directories(
  mcc PRIVATE
  ${CMAKE_SOURCE_DIR}/src/mcc
)

target_link_libraries(
  mcc PUBLIC
  fmt::fmt
)

set_target_properties(
  mcc PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  LINKER_LANGUAGE CXX
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
