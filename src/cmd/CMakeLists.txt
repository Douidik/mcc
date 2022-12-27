file(
  GLOB_RECURSE MCC_CMD_SOURCE
  ${MCC_FILE_REGEX} *.hpp
  ${MCC_FILE_REGEX} *.cpp
)

add_executable(
  mcc-cmd
  ${MCC_CMD_SOURCE}
)

target_include_directories(
  mcc-cmd PRIVATE
  ${CMAKE_SOURCE_DIR}/src/mcc
  ${CMAKE_SOURCE_DIR}/src/cmd
)

target_link_libraries(
  mcc-cmd PRIVATE
  mcc
)

set_target_properties(
  mcc-cmd PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
  LINKER_LANGUAGE CXX
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
