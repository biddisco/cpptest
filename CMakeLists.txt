cmake_minimum_required(VERSION 3.12)

project(cpptest C CXX)
message(STATUS "CMake system name is ${CMAKE_SYSTEM_NAME}")

# ------------------------------------------------------------------------------
# Policies - use the latest of everything
# ------------------------------------------------------------------------------
cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})

# ------------------------------------------------------------------------------
# Options
# ------------------------------------------------------------------------------
option(BUILD_SHARED_LIBS "Build shared libraries." ON)

# ------------------------------------------------------------------------------
# Setup Output directories
# ------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Single Directory for all Executables.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Single Directory for all Libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin
    CACHE PATH "Single Directory for all static libraries.")

# ------------------------------------------------------------------------------
# Reserved for future use
# ------------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# ------------------------------------------------------------------------------
# Enable testing
# ------------------------------------------------------------------------------
enable_testing()
include(CTest)

set(CMAKE_CXX_STANDARD 17)

# ------------------------------------------------------------------------------
# Make a dummy library to test shared linking
# ------------------------------------------------------------------------------
add_library(dummy_lib SHARED libtest.cxx)

# ------------------------------------------------------------------------------
# Simple test requiring no special libs
# ------------------------------------------------------------------------------
set(TEST_SRCS hello-cxx lock-cost)

# ------------------------------------------------------------------------------
# create exes for tests
# ------------------------------------------------------------------------------
foreach(test ${TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  target_include_directories(${test} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(${test} PRIVATE dummy_lib)
  add_test(NAME ${test} COMMAND ${test})
endforeach()

# ------------------------------------------------------------------------------
# Boost tests
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
add_subdirectory(boost)

# ------------------------------------------------------------------------------
# MPI tests
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
add_subdirectory(mpi)

# ------------------------------------------------------------------------------
# OpenGL tests
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
add_subdirectory(opengl)

# ------------------------------------------------------------------------------
# HPX tests
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
add_subdirectory(hpx)

# ------------------------------------------------------------------------------
# CUDA tests
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
add_subdirectory(cuda)

# ------------------------------------------------------------------------------
# HIP tests
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
# add_subdirectory(hip)

# ------------------------------------------------------------------------------
# CUDA + MPI tests
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
add_subdirectory(cuda+mpi)

# ------------------------------------------------------------------------------
# GPU test using whip for cuda/hip switching
# ------------------------------------------------------------------------------
#add_subdirectory(whip)
#add_subdirectory(gpu-whip)

# ------------------------------------------------------------------------------
# Jemalloc tests
# ------------------------------------------------------------------------------
# message(STATUS "-------------------------") add_subdirectory(jemalloc)

# ------------------------------------------------------------------------------
# . Mem usage test
# ------------------------------------------------------------------------------
# message(STATUS "-------------------------")
# add_subdirectory(mem)

# ------------------------------------------------------------------------------
# .
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")

if(0)
  find_package(PythonInterp)
  if(NOT PYTHONINTERP_FOUND)
    message(
      FATAL_ERROR
        "ERROR: Python interpreter not found. Cannot continue - sorry.")
  else()
    # check for pytest
    execute_process(
      COMMAND ${PYTHON_EXECUTABLE} -m pytest --version
      OUTPUT_VARIABLE PYTEST_output
      ERROR_VARIABLE PYTEST_error
      RESULT_VARIABLE PYTEST_result)
    if(${PYTEST_result})
      message(SEND_ERROR "Pytest package not available: ${PYTEST_error}")
    endif()
  endif()
endif()
