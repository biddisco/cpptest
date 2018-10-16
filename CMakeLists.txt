cmake_minimum_required(VERSION 2.8)

project (cpptest)

if(POLICY CMP0060)
  cmake_policy(SET CMP0060 NEW)
endif()

message("CMake system name is ${CMAKE_SYSTEM_NAME}")

OPTION (BUILD_SHARED_LIBS "Build shared libraries." OFF)

if(BGAS)
  include(BGAS-MVAPICH.cmake)
  include_directories(${MPI_INCLUDE_PATH})
endif()

find_package(MPI QUIET)
if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
  message("-------------------------\nMPI found version ${MPI_VERSION}\n-------------------------")
else()
  message("MPI was not found, no MPI tests will be built")
endif()

#------------------------------------------------------------------------------
# Boost
#------------------------------------------------------------------------------
set(Boost_ADDITIONAL_VERSIONS 1.59 1.60 1.61 1.62 1.63 1.64 1.65 1.66 1.67)
find_package( Boost 1.54.0 QUIET COMPONENTS thread atomic system program_options serialization filesystem regex context)
if(Boost_FOUND)
  message("-------------------------\nBoost found version ${Boost_VERSION}\n-------------------------")
else()
  message("Boost was not found, no Boost tests will be built")
endif()

option(USE_BOOST_DYNAMIC_LINK ON "Add BOOST_ALL_DYN_LINK to flags (windows usually)")

#------------------------------------------------------------------------------
# Make a dummy library to test shared linking
#------------------------------------------------------------------------------
add_library(dummy_lib SHARED libtest.cxx)

#------------------------------------------------------------------------------
# Simple test requiring no special libs
#------------------------------------------------------------------------------
set(TEST_SRCS
# simple c++test
  hello
)

#------------------------------------------------------------------------------
# Simple tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(BOOST_TEST_SRCS
# test boost serialize
  serialize
# test boost program options
  options
# test boost filesystem
  fs-tut1 fs-tut2 fs-tut3 fs-tut4 fs-tut5
# threads, atomic
  queue stack waitfreequeue atomicity atomic_ordering lockfree stdqueue
# regex
  regex-recursion
# accumulators/statistics
  histogram
# thread
  thread
# context
#  context
# Raspberry Pi bus test
  pi-lockfree
)

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

foreach(test ${TEST_SRCS})
  add_executable(${test} ${test}.cxx)
endforeach()

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIR})

  foreach(test ${BOOST_TEST_SRCS})
    add_executable(${test} ${test}.cxx)
    if (USE_BOOST_DYNAMIC_LINK AND ${test} MATCHES "options")
      set_target_properties(${test} PROPERTIES COMPILE_FLAGS "-DBOOST_ALL_DYN_LINK" )
    endif()
    target_link_libraries(${test} ${Boost_LIBRARIES} )
    if (CMAKE_COMPILER_IS_GNUCXX )
      target_link_libraries(${test} atomic )
    endif()
  endforeach()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${MPI_INCLUDE_PATH})

if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})

  foreach(test ${MPI_TEST_SRCS})
    add_executable(${test} ${test}.cxx)
    target_link_libraries(${test} ${MPI_C_LIBRARIES})
    set_property(TARGET ${test} PROPERTY COMPILE_DEFINITIONS ${MPI_CXX_COMPILE_DEFINITIONS})
  endforeach()
endif()
