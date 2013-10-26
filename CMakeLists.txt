PROJECT(CODE_TEST)

SET(CMAKE_CXX_COMPILER mpiicpc)
SET(CMAKE_C_COMPILER mpiicc)

cmake_minimum_required(VERSION 2.8)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O3 -g -openmp)
#set(CMAKE_EXE_LINKER_FLAGS -static)

SET(PRECISION double) #double float

# For home machine
#export BOOST_HOME=~/src/boost_1_54_0
#export CXX=icpc
#export CC=icc
#export PATH=$PATH:/opt/intel/composerxe-2011.4.191/bin/intel64
#export MKL_HOME=/opt/intel/composerxe-2011.4.191/mkl


# # find BOOST
SET(Boost_FOUND false)
if(Boost_INCLUDE_DIR)
  SET(Boost_FOUND true)
else()
  FIND_PATH(Boost_INCLUDE_DIR boost/config.hpp ${BOOST_HOME} ${BOOST_HOME}/include $ENV{BOOST_HOME} $ENV{BOOST_HOME}/include)
  FIND_PATH(Boost_LINK_DIR libboost_system.a ${BOOST_HOME} ${BOOST_HOME}/lib $ENV{BOOST_HOME} $ENV{BOOST_HOME}/lib)
  if(Boost_INCLUDE_DIR)
    SET(Boost_FOUND true)
  endif()
endif()
if(Boost_FOUND)
  SET(BOOST_LIBS boost_timer boost_system boost_mpi boost_serialization)
endif()

## # find LAPACK
#FIND_PATH(LAPACK_LINK_DIR liblapack.a )
#SET(LAPACK_FOUND false)
#if(LAPACK_LINK_DIR)
#  SET(LAPACK_FOUND true)
#else()
#  FIND_PATH(LAPACK_LINK_DIR liblapack.a /usr/lib ${LAPACK_HOME} ${LAPACK_HOME}/lib $ENV{LAPACK_HOME} $ENV{LAPACK_HOME}/lib)
#  if(LAPACK_LINK_DIR)
#    SET(LAPACK_FOUND true)
#  endif()
#endif()
#if(LAPACK_FOUND)
#  SET(LAPACK_LIBS lapack blas m )
#endif()



#FIND_PATH(MPI_INCLUDE_DIR mpi.h)
#FIND_PATH(MPI_LINK_DIR libmpich.a)
#SET(MPI_FOUND false)
#if(MPI_INCLUDE_DIR)
#  SET(MPI_FOUND true)
#else()
#  FIND_PATH(MPI_INCLUDE_DIR mpi.h /usr/include  /usr/include/mpich2)
#  FIND_PATH(MPI_LINK_DIR libmpich.a /usr/lib )
#  if(MPI_INCLUDE_DIR)
#    SET(MPI_FOUND true)
#  endif()
#endif()
#if(MPI_FOUND)
#  SET(MPI_LIBS mpich mpichcxx mpl )
#endif()

# FIND_PATH(NLOPT_INCLUDE_DIR nlopt.hpp)
# SET(NLOPT_FOUND false)
# if(NLOPT_INCLUDE_DIR)
#   SET(NLOPT_FOUND true)
# else()
#   FIND_PATH(NLOPT_INCLUDE_DIR nlopt.hpp ${NLOPT_HOME} ${NLOPT_HOME}/include $ENV{NLOPT_HOME} $ENV{NLOPT_HOME}/include)
#   FIND_PATH(NLOPT_LIB_DIR libnlopt_cxx.a ${NLOPT_HOME} ${NLOPT_HOME}/lib $ENV{NLOPT_HOME} $ENV{NLOPT_HOME}/lib)
#   if(NLOPT_INCLUDE_DIR)
#     SET(NLOPT_FOUND true)
#   endif()
# endif()

# find MKL
FIND_PATH(MKL_INCLUDE_DIR mkl.h)
FIND_PATH(MKL_LINK_DIR libmkl_core.a)
SET(MKL_FOUND false)
if(MKL_INCLUDE_DIR)
  SET(MKL_FOUND true)
else()
  FIND_PATH(MKL_INCLUDE_DIR mkl.h ${MKL_HOME} ${MKL_HOME}/include $ENV{MKL_HOME} $ENV{MKL_HOME}/include)
  FIND_PATH(MKL_LINK_DIR libmkl_core.a ${MKL_HOME} ${MKL_HOME}/lib/intel64  $ENV{MKL_HOME} $ENV{MKL_HOME}/lib/intel64)
  if(MKL_INCLUDE_DIR)
    SET(MKL_FOUND true)
  endif()
endif()
if(MKL_FOUND)
  SET(MKL_LIBS mkl_intel_lp64 mkl_intel_thread mkl_core iomp5 pthread m mkl_scalapack_lp64 mkl_blacs_intelmpi_lp64 )
endif()

INCLUDE_DIRECTORIES(${CODE_TEST_SOURCE_DIR}/src)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR} ${Boost_INCLUDE_DIR}/system ${Boost_INCLUDE_DIR}/timer)
#INCLUDE_DIRECTORIES(${MPI_INCLUDE_DIR})


LINK_DIRECTORIES(${MKL_LINK_DIR})
#LINK_DIRECTORIES(${MPI_LINK_DIR})
LINK_DIRECTORIES(${Boost_LINK_DIR})

SUBDIRS(src)
