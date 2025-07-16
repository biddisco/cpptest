# ------------------------------------------------------------------------------
# CUDA + MPI tests
# ------------------------------------------------------------------------------
find_package(CUDAToolkit)
find_package(MPI)
if((NOT MPI_FOUND) OR (NOT CUDAToolkit_FOUND))
  message(STATUS "CUDA+MPI tests not being built")
  return()
endif()

#------------------------------------------------------------------------------
# MPI tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(MPI_TEST_SRCS
  allgather_bug
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${MPI_TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  target_link_libraries(${test} MPI::MPI_CXX)
  target_link_libraries(${test} CUDA::cudart)

  add_test(NAME ${test}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS}
        $<TARGET_FILE:${test}> ${MPIEXEC_POSTFLAGS})
endforeach()

# ------------------------------------------------------------------------------
# Mini project for MPI_ping_pong
# ------------------------------------------------------------------------------
message(STATUS "-------------------------")
add_subdirectory(MPI_ping_pong)


