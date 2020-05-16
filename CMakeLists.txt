cmake_minimum_required(VERSION 3.6)

project(commandline)

set(LIB_NAME commandline)
set(EXE_NAME "")
set(TEST_EXE_NAME commandlinetest)

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(cmake/lib.cmake)

if(CCPM_BUILD_TEST)
include(cmake/npm.cmake)
  include(cmake/test.cmake)
  dp_require("@ccpm/cmocha")
  target_link_libraries(${TEST_EXE_NAME} cmocha)
endif()
