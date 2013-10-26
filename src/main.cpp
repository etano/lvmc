#ifndef MAIN
#define MAIN

#include <iostream>
#include <boost/mpi.hpp>

#include "config.h"
#include "util/inputFileStruct.h"
#include "linalg/distLinAlg.h"

namespace mpi = boost::mpi;

using namespace std;

int main(int argc, char* argv[])
{
  mpi::environment env(argc, argv);
  mpi::communicator world;

  control_block CB(argv[1]);
  if(!world.rank())
    CB.read();
  broadcast(world, CB, 0);
  
//   if(world.rank()==1)
//    CB.write(argv[2]);
  
  
  distLinAlg LA( world.size(), world.rank(), CB.p_grid[0], CB.p_grid[1] );
  
  int a_size(256);
  Tmatrix A(a_size,a_size);
  for(int i(0);i<a_size;++i)
    A(i,i)=1.0*i+1;
  
  LA.assignMatrix(A);
  LA.init();
  LA.invert();
  
  if(!world.rank())
  {
    for(int i(0);i<a_size;++i)
      std::cout<<A(i,i)<<" ";
    std::cout<<std::endl;
  }
  

  return 0;
};
#endif