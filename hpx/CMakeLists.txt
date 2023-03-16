#------------------------------------------------------------------------------
# HPX
#------------------------------------------------------------------------------
find_package(HPX QUIET)

if(HPX_FOUND)
  message(STATUS "HPX found version ${HPX_VERSION}")
else()
  message(STATUS "HPX was not found, no HPX tests will be built")
  return()
endif()

#------------------------------------------------------------------------------
# HPX tests
#------------------------------------------------------------------------------
set(HPX_TEST_SRCS
  hello-hpx
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${HPX_TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  target_link_libraries(${test} HPX::hpx)
endforeach()
