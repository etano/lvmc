#include <config.h>

#include "linalg/distLinAlg.h"


extern "C" {
  void blacs_pinfo_(int* mypnum, int* nprocs );
  void blacs_get_(int* icontxt, int* what, int* val );
  void blacs_gridinit_(int* icontxt, char* order, int* nprow, int* npcol);
  void blacs_gridinfo_(int* icontxt, int* nprow, int* npcol, int* myrow, int* mycol);
  
  void descinit_(int *idescal, int *m, int *n,int *mb,int *nb, int *dummy1 , int *dummy2 , int *icon, int *mla, int *info);
  int numroc_(int *n, int *nb, int *iproc, int *isrcprocs, int *nprocs);

#if PRECISION==double
  void pdpotrf_(const char* uplo, const int* n, double* a, const int* ia, const int* ja, const int* desca, int* info);
  void pdpotri_(const char* uplo, const int* n, double* a, const int* ia, const int* ja, const int* desca, int* info);
#elif PRECISION==single
  void pspotrf_(const char* uplo, const int* n, float* a, const int* ia, const int* ja, const int* desca, int* info);
  void pspotri_(const char* uplo, const int* n, float* a, const int* ia, const int* ja, const int* desca, int* info);
#endif
};

void distLinAlg::invertMatrixInPlace()
{
};


void distLinAlg::init()
{ 
  int hN(0);
  
  blacs_pinfo_( &hN, &nprocs );
  blacs_get_(  &hN,  &hN, &icontxt);
  blacs_gridinit_( &icontxt, &order, &nprow, &npcol ); 
  int mr(0),mc(0);
  blacs_gridinfo_( &icontxt, &nprow, &npcol, &mr, &mc ); 

  int info(0);
  int nB(128);
  int n(A->size1());
  
  int rN( std::max(1, numroc_( &n, &nB, &mr, &hN, &nprow) ) );
  int cN( std::max(1, numroc_( &n, &nB, &mc, &hN, &npcol) ) );
  
  desca.resize(9,0);
  descinit_( &desca[0], &n, &n, &nB, &nB, &hN, &hN,  &icontxt, &cN, &info );
//   std::copy(desca.begin(), desca.end(), std::ostream_iterator<int>(std::cout, "  "));
//   std::cout<<std::endl;
};
