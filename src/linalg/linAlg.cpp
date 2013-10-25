#include <config.h>
#include <numeric>  
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>
// #include <boost/numeric/ublas/matrix_proxy.hpp>

#include "linalg/linAlg.h"

#if PRECISION==double
extern "C" {
   
   void dgetrf(const int* n, const int* m, double* a, const int* n0,
              int* piv, int* st);
   void dgetri(const int* n, double* a, const int* n0,
              int* piv, double* work, const int* lwork, int* st);
   void dgesvd(char *JOBU, char* JOBVT, int *M, int *N,
              double *A, int *LDA, double *S, double *U,
              int *LDU, double *VT, int *LDVT, double *work,
              int *LWORK, int *INFO);
    void dsyev(const  char* JOBZ, const  char* UPLO,
              const int* N, double* A, const int* LDA, double* W,
              double* WORK, const int* LWORK, const int* INFO );
    void dgetrs(const char* TRANS, const int* N, const int* NRHS,
              double* A, const int* LDA,const int* IPIV,double* B,
              const int* LDB,const int* INFO);
};
#elif PRECISION==single
extern "C" {
  void sgetrf(const int* n, const int* m, float* a, const int* n0,
              int* piv, int* st);
  void sgetri(const int* n, float* a, const int* n0,
              int* piv, float* work, const int*, int* st);
  void sgesvd(char *JOBU, char* JOBVT, int *M, int *N,
              float *A, int *LDA, float *S, float *U,
              int *LDU, float *VT, int *LDVT, float *work,
              int *LWORK, int *INFO);
  void ssyev(const  char* JOBZ, const  char* UPLO,
              const int* N, float* A, const int* LDA, float* W,
              float* WORK, const int* LWORK, const int* INFO );
  void sgetrs(const char* TRANS, const int* N, const int* NRHS,
              float* A, const int* LDA,const int* IPIV, float* B,
              const int* LDB,const int* INFO);
}
#endif

namespace linalg
{
  
  void solveEigenSystem(Tmatrix& H, Tvector& Evalues)
  {
    char JOBZ('V');
    char UPLO('U');
    int N(H.size1());
    int LWORK(-1);
    int INFO(0);
    Tvector WORK(1);
    
#if PRECISION==double
    dsyev(&JOBZ,&UPLO,&N, &(H(0,0)) ,&N, &(Evalues[0]),&(WORK[0]),&LWORK,&INFO);
    WORK.resize(WORK[0]);
    LWORK=WORK[0];
    dsyev(&JOBZ,&UPLO,&N,&(H(0,0)),&N, &(Evalues[0]),&(WORK[0]),&LWORK,&INFO);
#elif PRECISION==single
    ssyev(&JOBZ,&UPLO,&N, &(H(0,0)) ,&N, &(Evalues[0]),&(WORK[0]),&LWORK,&INFO);
    WORK.resize(WORK[0]);
    LWORK=WORK[0];
    ssyev(&JOBZ,&UPLO,&N,&(H(0,0)),&N, &(Evalues[0]),&(WORK[0]),&LWORK,&INFO);
#endif
  };
  
  void LU_factorization(Tmatrix& A, Ivector& piv)
  {
    // <?>getrf( int matrix_order, lapack_int m, lapack_int n, <datatype>* a, lapack_int lda, lapack_int* ipiv );
    int m(A.size1());
    int n(A.size2());
    int info(0);
#if PRECISION==double
    dgetrf(  &m, &n, &(A(0,0)), &n, &(piv[0]),&info);
#elif PRECISION==single
    sgetrf(  &m, &n, &(A(0,0)), &n, &(piv[0]),&info);
#endif
  };
  
  void ls_nrm_solve(Tmatrix& A, Tvector& c)
  {
    // <?>getrs( int matrix_order, char trans, lapack_int n, lapack_int nrhs, const <datatype>* a, lapack_int lda, const lapack_int* ipiv, <datatype>* b, lapack_int ldb );
    int N(A.size1());
    int M(A.size2());
    char TRANS('N');
    int NRHS(1);
    Ivector IPIV(N,0);
    
    int info(0);
    LU_factorization(A,IPIV);
#if PRECISION==double
    dgetrs(  &TRANS, &N, &NRHS, &(A(0,0)), &N, &(IPIV[0]), &(c[0]), &N,&info);
#elif PRECISION==single
    sgetrs(  &TRANS, &N, &NRHS, &(A(0,0)), &N, &(IPIV[0]), &(c[0]), &N,&info);
#endif
    
  };
  
  // we use the boost libraries to invert the matrix
  void invertMatrix(Tmatrix& A, Tmatrix& B, RealType& det)
  {
    using namespace boost::numeric::ublas;
    typedef permutation_matrix<std::size_t> pmatrix;
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());
    // perform LU-factorization
    int res = lu_factorize(A,pm);
    det=1.0;
    for(int i(0); i<A.size1(); i++)
      det*=A(i,i);
    if( res != 0 )
      std::cerr<<"INVERT PROBLEM!"<<std::endl;
    // create identity matrix of "inverse"
      B.resize(A.size1(),A.size2());
      B.assign(ublas::identity_matrix<RealType>(A.size1()));
      // backsubstitute to get the inverse
      lu_substitute(A, pm, B);
  };
  
  
  // // we use the lapack libraries to invert the matrix
  void invertMatrixInPlace(Tmatrix& A, RealType& det)
  {
    int N=A.size1();
    int *IPIV = new int[N+1];
    int LWORK = N*N;
    double *WORK = new double[LWORK];
    int INFO;
    #if PRECISION==double
    dgetrf(&N,&N,&(A(0,0)),&N,IPIV,&INFO);
    det=0.0;
    for(int i(0); i<A.size1(); i++)
      det+=std::log(std::abs(A(i,i)));
    dgetri(&N,&(A(0,0)),&N,IPIV,WORK,&LWORK,&INFO);
    #elif PRECISION==single
    sgetrf(&N,&N,&(A(0,0)),&N,IPIV,&INFO);
    det=0.0;
    for(int i(0); i<A.size1(); i++)
      det+=std::log(std::abs(A(i,i)));
    sgetri(&N,&(A(0,0)),&N,IPIV,WORK,&LWORK,&INFO);
    #endif
    delete IPIV;
    delete WORK;
  };
  
  // dgesvd(jobu, jobvt, m, n, a, lda, s, u, ldu, vt, ldvt, work, lwork, info)
  void invertRankDeficientMatrix(Tmatrix& A, RealType& det)
  {
    int N=A.size1();
    int *IPIV = new int[N+1];
    int LWORK = -1;
    int INFO;
    char jobu('A');
    char jobvt('A');
    Tvector S(N,0);
    Tmatrix U(N,N);
    Tmatrix VT(N,N);
    #if PRECISION==double
    RealType *WORK = new double[1];
    dgesvd(&jobu, &jobvt, &N, &N, &(A(0,0)),&N,&(S[0]),&(U(0,0)),&N,&(VT(0,0)),&N,WORK,&LWORK,&INFO);
    LWORK=WORK[0];
    RealType *WORK2 = new double[LWORK];
    dgesvd(&jobu, &jobvt, &N, &N, &(A(0,0)),&N,&(S[0]),&(U(0,0)),&N,&(VT(0,0)),&N,WORK2,&LWORK,&INFO);
    #elif PRECISION==single
    RealType *WORK = new RealType[1];
    Sgesvd(&jobu, &jobvt, &N, &N, &(A(0,0)),&N,&(S[0]),&(U(0,0)),&N,&(VT(0,0)),&N,WORK,&LWORK,&INFO);
    LWORK=WORK[0];
    RealType *WORK2 = new RealType[LWORK];
    sgesvd(&jobu, &jobvt, &N, &N, &(A(0,0)),&N,&(S[0]),&(U(0,0)),&N,&(VT(0,0)),&N,WORK2,&LWORK,&INFO);
    #endif
    // //   compute "determinant" and invert S matrix
    det=0.0;
    RealType prec_S=N*S[0]*std::numeric_limits<RealType>::epsilon();
    //  T prec_S=std::numeric_limits<T>::epsilon();
    for(int i(0); i<N; i++)
      if(std::abs(S[i])>prec_S)
      {
        det+=std::log(std::abs(S[i]));
        S[i]=1.0/S[i];
      }
      else
      {
        det+=std::log(prec_S);
        S[i]=0.0;
      }
    U=trans(U);
    VT=trans(VT);
    for(int i(0); i<N; i++)
      for(int j(0); j<N; j++)
        U(i,j)*=S[j];
    ublas::axpy_prod(U,VT,A,true);
      
  };

};
