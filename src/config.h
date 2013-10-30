#ifndef CONFIG
#define CONFIG

#include <armadillo>
#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>

#ifdef USE_MPI
#include <mpi.h>
#endif

using namespace std;

#if PRECISION==double
typedef double RealType;
#elif PRECISION==single
typedef float RealType;
#endif

typedef arma::Mat<RealType> Tmatrix;
typedef arma::Col<RealType> Tvector;

typedef int I;
typedef arma::Mat<I> Imatrix;
typedef arma::Col<I> Ivector;

#endif
