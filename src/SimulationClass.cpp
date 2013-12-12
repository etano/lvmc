#include "SimulationClass.h"

void Simulation::ReadInput(string inFile)
{
  In.load(inFile);
}

void Simulation::BuildMPIModel()
{
  int N = WorldComm.NumProcs();
  int procsPerGroup = In.procsPerGroup;
  assert ((N % procsPerGroup) == 0);
  int nGroups = N/procsPerGroup;
  int myGroup = WorldComm.MyProc()/procsPerGroup;
  WorldComm.Split(myGroup, IntraComm);
  Ivector ranks(nGroups);
  for (int group=0; group<nGroups; group++)
    ranks(group) = group*procsPerGroup;
  WorldComm.Subset(ranks, InterComm);
  int nThreads = 1;
#if USE_OPENMP
  #pragma omp parallel
  {
    nThreads = omp_get_num_threads();
  }
#endif
  if (WorldComm.MyProc() == 0)
    cout <<"# Processes: "<<N<< ", # Groups: "<<nGroups
         <<", # Processes/Group: "<<procsPerGroup
         <<", # Threads/Process: "<<nThreads<<endl;
}

void Simulation::Run()
{
  Tests();
}


