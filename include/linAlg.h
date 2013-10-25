#ifndef LINALG
#define LINALG

#include <numeric>  
#include "config.h"

using namespace std;

namespace linalg
{
  void solveEigenSystem(Tmatrix& H, Tvector& Evalues);
  void ls_nrm_solve(Tmatrix& A, Tvector& c);
  void invertMatrix(Tmatrix& A, Tmatrix& B, RealType& det);
  void invertMatrixInPlace(Tmatrix& A, RealType& det);
  void invertRankDeficientMatrix(Tmatrix& A, RealType& det);
  RealType matrix_inner(const Tmatrix& A, const Tmatrix& B)
  {
    return std::inner_product(A.data().begin(), A.data().end(), B.data().begin(), 0);
  };
};
#endif
