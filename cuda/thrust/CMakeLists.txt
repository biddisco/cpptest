#------------------------------------------------------------------------------
# CUDA tests
#------------------------------------------------------------------------------
set(CUDA_TEST_SRCS
  sort
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${CUDA_TEST_SRCS})
  set(_args)
  add_executable(${test} ${test}.cu)
  target_include_directories(${test} PRIVATE ./Common)
  target_link_libraries(${test} CUDA::cublas)
  add_test(NAME ${test} COMMAND ${test} ${_args})
endforeach()
