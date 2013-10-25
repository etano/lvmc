#ifndef CONFIG
#define CONFIG

#include <cstring>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/property_tree/ptree.hpp>
// #include <boost/random.hpp>

namespace ublas = boost::numeric::ublas;


using namespace std;


#if PRECISION==double
typedef double RealType;
#elif PRECISION==single
typedef float RealType;
#endif

typedef ublas::matrix<RealType>  Tmatrix;
typedef ublas::vector<RealType>  Tvector;

typedef int I;
typedef ublas::matrix<I>  Imatrix;
typedef ublas::vector<I>  Ivector;

// typedef boost::mt19937 RNGType;

//
#endif