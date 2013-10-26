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
  
  
  distLinAlg LA( world.size(),world.rank(), CB.p_grid[0], CB.p_grid[1] );
  Tmatrix A(256,256);
  LA.assignMatrix(A);
  LA.init();
  

  return 0;
};
#endif