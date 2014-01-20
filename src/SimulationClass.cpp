#include "SimulationClass.h"

void Simulation::SetupIO(string inFile)
{
  // Input
  in.load(inFile);

  // Output
  string outputPrefix = in.getChild("IO").getAttribute<string>("outputPrefix");
  out.load(outputPrefix);
}

void Simulation::BuildMPIModel()
{
  procsPerGroup = in.getChild("Parallel").getAttribute<int>("procsPerGroup",1);
  int N = WorldComm.NumProcs();
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
  LatticeBuilder lb(in, out);
  Lattice* lat = lb.BuildLattice();
  lb.Report();
  cout << lat->T_ij << endl;
  cout << lat->nn_ij << endl;
}

