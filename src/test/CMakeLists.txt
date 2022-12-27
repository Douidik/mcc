file(
  GLOB_RECURSE MCC_TEST_SOURCE
  ${MCC_FILE_REGEX}*.hpp
  ${MCC_FILE_REGEX}*.cpp
)

add_executable(
  mcc-test
  ${MCC_TEST_SOURCE}
)

target_include_directories(
  mcc-test PRIVATE
  ${CMAKE_SOURCE_DIR}/src/mcc/
  ${CMAKE_SOURCE_DIR}/src/test/
)

target_link_libraries(
  mcc-test PRIVATE
  mcc
  gtest
)

set_target_properties(
  mcc-test PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  LINKER_LANGUAGE CXX
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
