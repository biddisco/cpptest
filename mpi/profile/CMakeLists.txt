#------------------------------------------------------------------------------
# Find MPI if possible, exit quietly if not found
#------------------------------------------------------------------------------
find_package(MPI REQUIRED)

#------------------------------------------------------------------------------
# MPI tests which have one c++ file per exe
#------------------------------------------------------------------------------
set(MPI_PROFILE_SRCS
  example.cxx
  profile.cxx
)

#------------------------------------------------------------------------------
# create exes for tests
#------------------------------------------------------------------------------
add_executable(mpi_profile ${MPI_PROFILE_SRCS})
target_link_libraries(mpi_profile MPI::MPI_CXX)

add_test(NAME mpi_profile
  COMMAND ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} 2 ${MPIEXEC_PREFLAGS}
      $<TARGET_FILE:mpi_profile> ${MPIEXEC_POSTFLAGS})
