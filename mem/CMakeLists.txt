message(STATUS "-------------------------")

# Simple tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(MEM_TEST_SRCS
    mem_use
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${MEM_TEST_SRCS})
  set(_args)
  add_executable(${test} ${test}.cxx)

  add_test(NAME ${test} COMMAND ${test} ${_args})
endforeach()
