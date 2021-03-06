PROJECT(ARMA_TEST)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#if you don't want the full compiler output, set to "OFF"
SET(CMAKE_VERBOSE_MAKEFILE OFF)

#-------------------------------------------------------------------
# Options
#-------------------------------------------------------------------

SET(PRECISION double) #double float
SET(USE_MPI TRUE)
SET(USE_OPENMP TRUE)
SET(QMC_BUILD_STATIC FALSE)
SET(NDIM 2)
SET(COMPILER_MAKE "INTEL") # INTEL, GNU, or IBM

#-------------------------------------------------------------------
# Compilers
#-------------------------------------------------------------------
IF(${COMPILER_MAKE} MATCHES "IBM")
  # set the compiler
  IF(USE_MPI)
    SET(CMAKE_C_COMPILER mpixlc_r)
    SET(CMAKE_CXX_COMPILER mpixlcxx_r)
  ELSE()
    SET(CMAKE_C_COMPILER xlc_r)
    SET(CMAKE_CXX_COMPILER xlc++_r)
  ENDIF()

  SET(AIX_ARCH "qp")
  SET(AIX_ARCH_FLAGS "-qarch=${AIX_ARCH} -qstrict -qhot=level=1 -qtune=qp -qsimd=auto")
  IF(USE_OPENMP)
    SET(AIX_ARCH_FLAGS "${AIX_ARCH_FLAGS} -qsmp=omp -qthreaded")
  ENDIF()

  SET(AIX_CXX_COMMON_FLAGS "-qinline=auto:level=10")
  SET(AIX_OPT_FLAGS "-qmaxmem=-1")

  SET(CMAKE_CXX_FLAGS "${AIX_ARCH_FLAGS} ${AIX_CXX_COMMON_FLAGS} ${AIX_OPT_FLAGS}")
  SET(CMAKE_C_FLAGS "${AIX_ARCH_FLAGS} ${AIX_OPT_FLAGS}")
ENDIF()

IF(${COMPILER_MAKE} MATCHES "INTEL")
  IF(USE_MPI)
    SET(CMAKE_C_COMPILER mpicc)
    SET(CMAKE_CXX_COMPILER mpic++)
  ELSE()
    SET(CMAKE_C_COMPILER icc)
    SET(CMAKE_CXX_COMPILER icpc)
  ENDIF()

  SET(INTEL_C_FLAGS "-w")
  IF(USE_OPENMP)
    SET(INTEL_C_FLAGS "${INTEL_C_FLAGS} -fopenmp")
  ENDIF(USE_OPENMP)

  SET(CMAKE_CXX_FLAGS "${INTEL_C_FLAGS}")
  SET(CMAKE_C_FLAGS "${INTEL_C_FLAGS}")
ENDIF()

IF(${COMPILER_MAKE} MATCHES "GNU")
  IF(USE_MPI)
    SET(CMAKE_C_COMPILER mpicc)
    SET(CMAKE_CXX_COMPILER mpicxx)
  ELSE()
    SET(CMAKE_C_COMPILER cc)
    SET(CMAKE_CXX_COMPILER c++)
  ENDIF()

  SET(GNU_C_FLAGS "-w")
  IF(USE_OPENMP)
    SET(GNU_C_FLAGS "${GNU_C_FLAGS} -fopenmp")
  ENDIF(USE_OPENMP)

  SET(CMAKE_CXX_FLAGS "${GNU_C_FLAGS}")
  SET(CMAKE_C_FLAGS "${GNU_C_FLAGS}")
ENDIF()

SET(COMMON_FLAGS "-O3 -g -DNDIM=${NDIM}")
IF(USE_MPI)
  SET(COMMON_FLAGS "${COMMON_FLAGS} -DUSE_MPI")
ENDIF(USE_MPI)
IF(USE_OPENMP)
  SET(COMMON_FLAGS "${COMMON_FLAGS} -DUSE_OPENMP")
ENDIF(USE_OPENMP)

SET(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")
SET(CMAKE_CXX_SOURCE_FILE_EXTENSIONS ,"cpp")

SET(CMAKE_C_FLAGS "-std=c99 ${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
SET(CMAKE_C_SOURCE_FILE_EXTENSIONS ,"c")

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

# Find Lapack/Blas/Atlas/MKL/XLmass/ESSL/...
SET(LAPACKLIBS "")
IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "ppc") # This covers GNU and IBM Compilers on BGQ
  # Find Lapack/BLAS BGQ
  SET(LAPACK_LIBRARY /usr/local/tools/lapack/lib/liblapack.a)
  SET(BLAS_LIBRARY /usr/local/tools/blas/lib/libblas.a)
  SET(ESSL_LIBRARY /usr/local/tools/essl/5.1/lib/libesslsmpbg.a)
  SET(XLMASS_LIBRARY /opt/ibmcmp/xlmass/bg/7.3/bglib64/libmass.a
                     /opt/ibmcmp/xlmass/bg/7.3/bglib64/libmassv.a)
  SET(FORTRAN_LIBRARY /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlf90_r.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxl.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlfmath.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlfpmt4.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlfpmt8.a
                      /opt/ibmcmp/xlf/bg/14.1/bglib64/libxlopt.a)
  SET(LAPACK_LIBS ${LAPACK_LIBRARY} ${BLAS_LIBRARY} ${ESSL_LIBRARY}
    ${XLMASS_LIBRARY} ${FORTRAN_LIBRARY} -ldl)
ELSE()
  IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "INTEL")
    # Find MKL
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
    SET(LAPACK_LIBS ${MKL_LIBS})
  ENDIF()
ENDIF()

# Find Armadillo
SET(ARMA_INCLUDE_DIR $ENV{ARMA_HOME}/include)
SET(ARMA_LIB_DIR $ENV{ARMA_HOME}/lib64)
SET(ARMA_LIBS armadillo)
INCLUDE_DIRECTORIES(${ARMA_INCLUDE_DIR})
LINK_DIRECTORIES(${ARMA_LIB_DIR})

# Find HDF5
SET(HDF5_INCLUDE_DIR $ENV{HDF5_HOME}/include)
SET(HDF5_LIB_DIR $ENV{HDF5_HOME}/lib64)
SET(HDF5_LIBS hdf5 hdf5_cpp)
INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIR})
LINK_DIRECTORIES(${HDF5_LIB_DIR})

# Create target
SET (TEMPLATE_SRCS src/main.cpp
  src/Utils/Communication/Communication.cpp
  src/Utils/IO/InputClass.cpp
  src/Utils/IO/xmlParser.cpp
  src/Utils/RNG/RNGClass.cpp
  src/Utils/RNG/sfmt.cpp
  src/Utils/RNG/mersenne.cpp
  src/Utils/RNG/userintf.cpp
  src/Utils/RNG/stoc1.cpp
  src/LatticeBuilderClass.cpp
  src/SimulationClass.cpp)
ADD_EXECUTABLE(lvmc ${TEMPLATE_SRCS})
TARGET_LINK_LIBRARIES(lvmc ${LAPACK_LIBS} ${ARMA_LIBS} ${HDF5_LIBS})

# Install to home directory
INSTALL(TARGETS lvmc DESTINATION $ENV{HOME}/bin)
