PROJECT(ARMADILLO_TEST)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE OFF)

#-------------------------------------------------------------------
# Options
#-------------------------------------------------------------------

SET(PRECISION double) #double float
SET(USE_MPI TRUE)
SET(QMC_BUILD_STATIC FALSE)
SET(NDIM 2)

#-------------------------------------------------------------------
# Compilers
#-------------------------------------------------------------------
SET(C_FLAGS "-DH5_USE_16_API -O3 -msse3 -w -DNDIM=${NDIM}")
IF(USE_MPI)
  SET(C_FLAGS "${C_FLAGS} -DUSE_MPI -fopenmp")
ENDIF(USE_MPI)

SET(CMAKE_CXX_COMPILER,"CC")
SET(CMAKE_CXX_FLAGS "${C_FLAGS}")
SET(CMAKE_CXX_SOURCE_FILE_EXTENSIONS ,"cc")

SET(CMAKE_C_COMPILER,"cc")
SET(CMAKE_C_FLAGS "-std=c99 ${C_FLAGS}")
SET(CMAKE_C_SOURCE_FILE_EXTENSIONS ,"c")

SET(CMAKE_Fortran_COMPILER,"ftn")
SET(CMAKE_Fortran_FLAGS "-funroll-loops -O3")
SET(CMAKE_Fortran_SOURCE_FILE_EXTENSIONS ,"f")

IF(QMC_BUILD_STATIC)
  # Enable static linking
  set(BUILD_STATIC_LIBS TRUE)
  set(LINK_SEARCH_END_STATIC TRUE)
  set(CMAKE_EXE_LINKER_FLAGS "-static -Wl,-Bstatic")
  set(MPI_LINK_FLAGS "-Wl,-Bstatic")
  set(LINK_SEARCH_END_STATIC TRUE)
  set(HDF5_USE_STATIC_LIBRARIES TRUE)

  # Disable shared linking
  set(BUILD_SHARED_LIBS FALSE)
  set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
  set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
  set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
  set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
  set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
  set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
ENDIF(QMC_BUILD_STATIC)

#-------------------------------------------------------------------
# Directories
#-------------------------------------------------------------------
SET(LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib" CACHE PATH "Single output directory for building all libraries")
SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin" CACHE PATH "Single output directory for building all executables")
SET(CMAKE_INSTALL_PREFIX "$ENV{HOME}" CACHE PATH "Default install to home directory")
SET(PROJECT_CMAKE "${CMAKE_SOURCE_DIR}/CMake")
SET(BUILD_DIR "${CMAKE_SOURCE_DIR}/build")

#-------------------------------------------------------------------
# Dependencies
#-------------------------------------------------------------------

# find MKL
FIND_PATH(MKL_INCLUDE_DIR mkl.h)
FIND_PATH(MKL_LINK_DIR libmkl_core.a)
SET(MKL_FOUND false)
IF(MKL_INCLUDE_DIR)
  SET(MKL_FOUND true)
ELSE()
  FIND_PATH(MKL_INCLUDE_DIR mkl.h ${MKL_HOME} ${MKL_HOME}/include $ENV{MKL_HOME} $ENV{MKL_HOME}/include)
  FIND_PATH(MKL_LINK_DIR libmkl_core.a ${MKL_HOME} ${MKL_HOME}/lib/intel64  $ENV{MKL_HOME} $ENV{MKL_HOME}/lib/intel64)
  IF(MKL_INCLUDE_DIR)
    SET(MKL_FOUND true)
  ENDIF()
ENDIF()
IF(MKL_FOUND)
  SET(MKL_LIBS mkl_intel_lp64 mkl_intel_thread mkl_core iomp5 pthread m mkl_scalapack_lp64 mkl_blacs_intelmpi_lp64)
  #SET(MKL_LIBS "-L$ENV{MKL_HOME};mkl_intel_lp64;mkl_sequential;mkl_core")
ENDIF()
LINK_DIRECTORIES(${MKL_LINK_DIR})

# find armadillo
SET(ARMADILLO_INCLUDE_DIRS $ENV{ARMA_HOME}/include)
SET(ARMADILLO_LIBRARY_DIRS $ENV{ARMA_HOME}/lib64)
SET(ARMADILLO_LIBRARIES armadillo)
INCLUDE_DIRECTORIES(${ARMADILLO_INCLUDE_DIRS})
LINK_DIRECTORIES(${ARMADILLO_LIBRARY_DIRS})

SET (ARMADILLO_TEST_SRCS src/armadillo_test.cpp
  src/Communication/Communication.cpp src/MPITests.cpp)
ADD_EXECUTABLE(armadillo_test ${ARMADILLO_TEST_SRCS})
TARGET_LINK_LIBRARIES(armadillo_test ${MKL_LIBS} ${ARMADILLO_LIBRARIES})

INSTALL(TARGETS armadillo_test DESTINATION ${ARMADILLO_TEST_SOURCE_DIR}/bin)
