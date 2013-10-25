#ifndef MAIN
#define MAIN

#include <iostream>
#include <boost/mpi.hpp>

#include "config.h"
#include "util/inputFileStruct.h"

namespace mpi = boost::mpi;

using namespace std;

int main(int argc, char* argv[])
{
  mpi::environment env(argc, argv);
  mpi::communicator world;
  
  control_block CB;
  if(world.rank()==0)
    CB.read(argv[1]);
  
  broadcast(world, CB, 0);
  
  if(world.rank()==1)
   CB.write(argv[2]);
  
  MPI_Finalize();  
  return 0;
};
#endif