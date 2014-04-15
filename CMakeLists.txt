cmake_minimum_required(VERSION 2.8)

project (cpptest)

OPTION (BUILD_SHARED_LIBS "Build shared libraries." OFF)

include(BGAS-MVAPICH.cmake)
include_directories(${MPI_INCLUDE_PATH})
#------------------------------------------------------------------------------
# Boost
#------------------------------------------------------------------------------
find_package( Boost 1.54.0 REQUIRED thread system program_options serialization filesystem regex)
include_directories(${Boost_INCLUDE_DIR})
if(Boost_FOUND)
  message("Boost found version ${Boost_VERSION}")
endif()

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
LINK_DIRECTORIES(${Boost_LIBRARY_DIR})

#------------------------------------------------------------------------------
# Make a dummy library to test shared linking
#------------------------------------------------------------------------------
add_library(test SHARED libtest.cxx)

#------------------------------------------------------------------------------
# Simple tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(TEST_SRCS 
# simple c++test
  hello 
# test boost serialize
  serialize 
# test boost program options
  options 
# test boost filesystem
  fs-tut1 fs-tut2 fs-tut3 fs-tut4 fs-tut5 
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------

foreach(test ${TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  target_link_libraries(${test} ${MPI_C_LIBRARIES})
  target_link_libraries(${test} ${Boost_LIBRARIES})
endforeach()

