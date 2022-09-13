#------------------------------------------------------------------------------
# Find MPI if possible, exit quietly if not found
#------------------------------------------------------------------------------
find_package(MPI QUIET)

message(STATUS "-------------------------")

if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
  message(STATUS "MPI found version ${MPI_CXX_VERSION}")
else()
  message(STATUS "MPI was not found, no MPI tests will be built")
  return()
endif()

#------------------------------------------------------------------------------
# MPI tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(MPI_TEST_SRCS
  hello-mpi
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
foreach(test ${MPI_TEST_SRCS})
  add_executable(${test} ${test}.cxx)
  target_link_libraries(${test} MPI::MPI_CXX)

  add_test(NAME ${test}
    COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS}
        $<TARGET_FILE:${test}> ${MPIEXEC_POSTFLAGS})
endforeach()
