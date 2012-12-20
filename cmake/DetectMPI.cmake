#######################################################################
# find MPI compiler or libraries

set(MPI_C_FIND_QUIETLY ON)
set(MPI_CXX_FIND_QUIETLY ON)
set(MPI_Fortran_FIND_QUIETLY ON)

find_package( MPI QUIET ) # Use the standard CMake FindMPI

if( MPI_CXX_COMPILER )
  coolfluid_log_file( "[MPI] Already using MPI C++ compiler, no need of MPI libraries." )
  coolfluid_log_file( "     MPI CXX COMPILER   : [${CMAKE_CXX_COMPILER}]")
else()
  coolfluid_log_file( "[MPI] No MPI C++ compiler was set. Must find MPI libraries ..." )
endif()

coolfluid_log_file( "     MPI_INCLUDE_PATH   : [${MPI_INCLUDE_PATH}]")
coolfluid_log_file( "     MPI_LIBRARIES      : [${MPI_LIBRARIES}]")
if( MPI_CXX_LIBRARIES )
    coolfluid_log_file( "     MPI_CXX_LIBRARIES      : [${MPI_CXX_LIBRARIES}]")
endif()

###############################################################################
# check that MPI was found

if(MPI_CXX_COMPILER OR MPI_COMPILER)
  set( CF3_HAVE_MPI 1 CACHE BOOL "Found MPI compiler" )
else()
  if( MPI_CXX_FOUND )
    set( CF3_HAVE_MPI 1 CACHE BOOL "User enabled MPI [FOUND]" )
  else()
    message( FATAL_ERROR "[MPI] no MPI compiler or libraries were found.\n   MPI is required to compile coolfluid." )
  endif()
endif()

#######################################################################
# add MPI include path

# if mpi was found add it to the include path if needed
if( CF3_HAVE_MPI AND (NOT MPI_CXX_COMPILER OR CF3_MPI_USE_HEADERS) )
  include_directories( ${MPI_INCLUDE_PATH} )
  list( APPEND CF3_DEPS_LIBRARIES ${MPI_LIBRARIES} )
  message("Explicitly added mpi headers: ${MPI_INCLUDE_PATH}")
endif()

set(CF3_MPIRUN_PROGRAM ${MPIEXEC} CACHE FILEPATH "Filepath to the MPIRUN program file")
coolfluid_log_file( "     CF3_MPIRUN_PROGRAM : [${CF3_MPIRUN_PROGRAM}]" )

mark_as_advanced( CF3_HAVE_MPI CF3_MPIRUN_PROGRAM )

coolfluid_set_package( PACKAGE MPI DESCRIPTION "parallel communication"
                       TYPE REQUIRED
                       QUIET
                       )

