# this dir is from 
# https://github.com/olcf-tutorials/MPI_ping_pong.git
# commit 0326b1d57784ab3c407c3c7857292770fca10e7c 

cmake_policy(SET CMP0104 NEW) # Set CMAKE_CUDA_ARCHITECTURES for NVidia compiler

#------------------------------------------------------------------------------
# CUDA Language support
#------------------------------------------------------------------------------
include(CheckLanguage)
check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
  enable_language(CUDA)
else()
  message(STATUS "No CUDA support")
  return()
endif()

#------------------------------------------------------------------------------
# Check Find CudaToolkit works ok
#------------------------------------------------------------------------------
include(FindCUDAToolkit)
find_package(CUDAToolkit)

if (CUDAToolkit_FOUND)
  message(STATUS "CUDAToolkit found")
else()
  message(WARNING "CUDAToolkit NOT found")
endif()

#------------------------------------------------------------------------------
# Check MPI
#------------------------------------------------------------------------------
find_package(MPI QUIET)

if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
  message(STATUS "MPI found version ${MPI_CXX_VERSION}")
else()
  message(STATUS "MPI was not found, no MPI tests will be built")
  return()
endif()

#------------------------------------------------------------------------------
# CUDA tests
#------------------------------------------------------------------------------
set(CUDA_TEST_SRCS
  cpu/ping_pong.c
  cuda_staged/ping_pong_cuda_staged.cu
  cuda_aware/ping_pong_cuda_aware.cu
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${CUDA_TEST_SRCS})
  string(REGEX REPLACE ".*/"   "" test_name ${test})
  message(STATUS "test_name: ${test_name}")

  string(REGEX REPLACE "\\..*" "" test_name ${test_name})
  message(STATUS "test_name: ${test_name}")

  set(test_name mpi_${test_name})
  message(STATUS "test_name: ${test_name}")
  message(STATUS "test: ${test}")
  
  if ("${test}" MATCHES "\\.c$")
    string(REPLACE ".c" "" test_name ${test_name})
    add_executable(${test_name} ${test})
  elseif ("${test}" MATCHES "\\.cu$")
    string(REPLACE ".cu" "" test_name ${test_name})
    add_executable(${test_name} ${test})
  endif()
  set(_args)
  target_link_libraries(${test_name} MPI::MPI_CXX)
  add_test(NAME ${test_name}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS}
        $<TARGET_FILE:${test_name}> ${MPIEXEC_POSTFLAGS})
endforeach()

