cmake_minimum_required(VERSION 3.12)

project (cpptest C CXX)
message("CMake system name is ${CMAKE_SYSTEM_NAME}")

#------------------------------------------------------------------------------
# Policies
#------------------------------------------------------------------------------
cmake_policy(SET CMP0028 NEW) # Double colon in target name means ALIAS or IMPORTED target
cmake_policy(SET CMP0060 NEW) # Link libraries by full path even in implicit directories
cmake_policy(SET CMP0074 NEW) # find_package uses XXX_ROOT vars

#------------------------------------------------------------------------------
# Options
#------------------------------------------------------------------------------
OPTION (BUILD_SHARED_LIBS "Build shared libraries." ON)

#------------------------------------------------------------------------------
# Setup Output directories
#------------------------------------------------------------------------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
)
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Libraries"
)
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all static libraries."
)

#------------------------------------------------------------------------------
# Reserved for future use
#------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#------------------------------------------------------------------------------
# Enable testing
#------------------------------------------------------------------------------
enable_testing()
include(CTest)

#------------------------------------------------------------------------------
# Make a dummy library to test shared linking
#------------------------------------------------------------------------------
add_library(dummy_lib SHARED libtest.cxx)

#------------------------------------------------------------------------------
# Simple test requiring no special libs
#------------------------------------------------------------------------------
set(TEST_SRCS
  hello-cxx
  lock-cost
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${test} PRIVATE dummy_lib)
  add_test(NAME ${test} COMMAND ${test})
endforeach()

#------------------------------------------------------------------------------
# Boost tests
#------------------------------------------------------------------------------
add_subdirectory(boost)

#------------------------------------------------------------------------------
# MPI tests
#------------------------------------------------------------------------------
add_subdirectory(mpi)

#------------------------------------------------------------------------------
# HPX tests
#------------------------------------------------------------------------------
add_subdirectory(hpx)

#------------------------------------------------------------------------------
# CUDA tests
#------------------------------------------------------------------------------
add_subdirectory(cuda)
