set(Boost_USE_STATIC_LIBS ON)

#------------------------------------------------------------------------------
# Boost
#------------------------------------------------------------------------------
find_package( Boost 1.54.0 QUIET COMPONENTS thread atomic system program_options serialization context unit_test_framework)
if(Boost_FOUND)
  message(STATUS "Boost found version ${Boost_VERSION}")
  message(STATUS "Boost libraries are ${Boost_LIBRARIES}")
else()
  message(STATUS "Boost was not found, no Boost tests will be built")
  return()
endif()

#------------------------------------------------------------------------------
# OPtions
#------------------------------------------------------------------------------
option(USE_BOOST_DYNAMIC_LINK OFF "Add BOOST_ALL_DYN_LINK to flags (windows usually)")

#------------------------------------------------------------------------------
# Simple tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(BOOST_TEST_SRCS
# test boost serialize
  serialize
# test boost program options
  options
# threads, atomic
  queue stack waitfreequeue atomicity atomic_ordering lockfree stdqueue
# accumulators/statistics
  histogram
# thread
  thread
# context
  test_fiber
# Raspberry Pi bus test
  pi-lockfree
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${BOOST_TEST_SRCS})
  set(_args)
  add_executable(${test} ${test}.cxx)

  if (${test} MATCHES "options")
  set(_args -i 3 1 2 3 3 3 6 4 -j 1 2 3 4 5)
  endif()
  if (USE_BOOST_DYNAMIC_LINK AND ${test} MATCHES "options")
    target_compile_definitions(${test} PRIVATE "BOOST_TEST_DYN_LINK=1")
  endif()
  if (USE_BOOST_DYNAMIC_LINK AND ${test} MATCHES "fiber")
  target_compile_definitions(${test} PRIVATE "BOOST_TEST_DYN_LINK=1")
  endif()
  target_link_libraries(${test} ${Boost_LIBRARIES})
  target_include_directories(${test} PRIVATE ${Boost_INCLUDE_DIRS})

  # indicates the shared library variant
  # indicates the link paths
  target_link_libraries(${test} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  add_definitions(-DBOOST_TEST_DYN_LINK)

#  if (CMAKE_COMPILER_IS_GNUCXX )
#  target_link_libraries(${test} atomic)
#  endif()
  add_test(NAME ${test} COMMAND ${test} ${_args})
endforeach()
