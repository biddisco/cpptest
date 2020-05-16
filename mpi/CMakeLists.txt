#------------------------------------------------------------------------------
# Find MPI if possible, exit quietly if not found
#------------------------------------------------------------------------------
find_package(MPI QUIET)

if(MPI_FOUND)
  include_directories(${MPI_INCLUDE_PATH})
  message("-------------------------\nMPI found version ${MPI_CXX_VERSION}\n-------------------------")
else()
  message("MPI was not found, no MPI tests will be built")
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
endforeach()
