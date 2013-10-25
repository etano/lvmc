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
  
  
  std::cout<<world.rank()<<" "<<world.size()<<std::endl;
  
  return 0;
};
#endif