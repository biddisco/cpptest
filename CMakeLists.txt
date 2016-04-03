cmake_minimum_required(VERSION 2.8)

project (cpptest)

message("CMake system name is ${CMAKE_SYSTEM_NAME}")

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
# Boost
#------------------------------------------------------------------------------
find_package( Boost 1.54.0 REQUIRED thread atomic system program_options serialization filesystem regex context)
if(Boost_FOUND)
  message("Boost found version ${Boost_VERSION}")
endif()

option(USE_BOOST_DYNAMIC_LINK ON "Add BOOST_ALL_DYN_LINK to flags (windows usually)")

#------------------------------------------------------------------------------
# Make a dummy library to test shared linking
#------------------------------------------------------------------------------
add_library(test SHARED libtest.cxx)

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
  queue stack waitfreequeue
# regex
  regex-recursion
# context
  context
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

include_directories(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIR})

foreach(test ${BOOST_TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  if (USE_BOOST_DYNAMIC_LINK AND ${test} MATCHES "options")
    set_target_properties(${test} PROPERTIES COMPILE_FLAGS "-DBOOST_ALL_DYN_LINK" )
  endif()
  target_link_libraries(${test} ${Boost_LIBRARIES} atomic)
endforeach()

include_directories(${MPI_INCLUDE_PATH})

if(MPI_FOUND)
  foreach(test ${MPI_TEST_SRCS})
    add_executable(${test} ${test}.cxx)
    target_link_libraries(${test} ${MPI_C_LIBRARIES})
  endforeach()
endif()
