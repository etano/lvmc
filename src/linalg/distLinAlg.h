#ifndef DISTLINALG
#define DISTLINALG

#include "config.h"

using namespace std;

class distLinAlg {
  
  public:
    distLinAlg(int np, int me, int nr, int nc): order('R'), mypnum(me), nprocs(np), icontxt(0), what(0), nprow(nr), npcol(nc)
    {
    };
    
    ~distLinAlg(){};
    void invertMatrixInPlace();
    void assignMatrix(Tmatrix& a)
    {
      A = &a;      
    };
    
    void init();
    
  private:
    char order;
    int mypnum, nprocs, icontxt, what, val, nprow, npcol;
    vector<int> desca;
    Tmatrix* A;
};
#endif
