set(CPM_VERSION 0.35.0)
set(CPM_SOURCE "https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_VERSION}/CPM.cmake")
set(CPM_FILEPATH "${CMAKE_SOURCE_DIR}/cmake/cpm_v${CPM_VERSION}.cmake")

if(NOT EXISTS ${CPM_FILEPATH})
  file(DOWNLOAD ${CPM_SOURCE} ${CPM_FILEPATH})
endif()

include(${CPM_FILEPATH})

CPMAddPackage("gh:google/googletest#release-1.12.1")
CPMAddPackage("gh:neargye/magic_enum#v0.8.1")
CPMAddPackage("gh:fmtlib/fmt#8.1.1")
