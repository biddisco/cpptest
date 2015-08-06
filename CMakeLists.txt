cmake_minimum_required(VERSION 2.8)

project (mpitest)

OPTION (BUILD_SHARED_LIBS "Build shared libraries." OFF)

if(BGAS)
  include(BGAS-MVAPICH.cmake)
  include_directories(${MPI_INCLUDE_PATH})
endif()

find_package(MPI)
if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
endif()

#------------------------------------------------------------------------------
# MPI tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(MPI_TEST_SRCS 
# simple mpi c++test
  hellompi
) 

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
if(MPI_FOUND)
  foreach(test ${MPI_TEST_SRCS})
    add_executable(${test} ${test}.cxx)
    target_link_libraries(${test} ${MPI_C_LIBRARIES})
  endforeach()
endif()
