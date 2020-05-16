#------------------------------------------------------------------------------
# Threading libs needed on linux
#------------------------------------------------------------------------------
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  # Linux specific code
  #  set(THREAD_LIBRARIES pthread)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

#------------------------------------------------------------------------------
# Boost
#------------------------------------------------------------------------------
find_package( Boost 1.54.0 QUIET COMPONENTS thread atomic system program_options serialization filesystem regex context)
if(Boost_FOUND)
  message("-------------------------\nBoost found version ${Boost_VERSION}\n-------------------------")
  set(PROJECT_Boost_LIBRARIES ${Boost_LIBRARIES})
else()
  message("Boost was not found, no Boost tests will be built")
  return()
endif()

#------------------------------------------------------------------------------
# OPtions
#------------------------------------------------------------------------------
option(USE_BOOST_DYNAMIC_LINK ON "Add BOOST_ALL_DYN_LINK to flags (windows usually)")

message("Boost libraries are ${Boost_LIBRARIES}")

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
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${BOOST_TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  if (USE_BOOST_DYNAMIC_LINK AND ${test} MATCHES "options")
    set_target_properties(${test} PROPERTIES COMPILE_FLAGS "-DBOOST_ALL_DYN_LINK" )
  endif()
  target_link_libraries(${test} ${PROJECT_Boost_LIBRARIES} ${THREAD_LIBRARIES})
  target_include_directories(${test} PRIVATE ${Boost_INCLUDE_DIRS})
  if (CMAKE_COMPILER_IS_GNUCXX )
    target_link_libraries(${test} atomic )
  endif()
endforeach()
