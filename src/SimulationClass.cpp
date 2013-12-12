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
  HDF5Tests();
  MPITests();
}

void Simulation::HDF5Tests()
{
  // HDF5 Tests
  if (WorldComm.MyProc() == 0) {
    cout <<"Writing to HDF5 file..."<<endl;
    H5::H5File file("test.h5", H5F_ACC_TRUNC);
    Tmatrix A = arma::randu<Tmatrix>(2,2);
    cout << A << endl;
    IOClass IOHDF5;
    IOHDF5.Write(file, "doubleArray", A);
    cout <<"Reading from HDF5 file..."<<endl;
    Tmatrix B = arma::randu<Tmatrix>(2,2);
    IOHDF5.Read(file, "doubleArray", B);
    cout << B << endl;
  }
  cin.ignore();
  WorldComm.BarrierSync(); // Sync procs within each clone.

  if (WorldComm.MyProc() == 0) {
    H5::H5File file("test.h5", H5F_ACC_TRUNC);
    IOClass IOHDF5;
    IOHDF5.CreateGroup(file, "Data");
    Tmatrix A = arma::randu<Tmatrix>(3,2);
    cout << A << endl;
    IOHDF5.CreateExtendableDataSet(file, "/Data/", "test", A);
    Tmatrix B = arma::randu<Tmatrix>(3,2);
    cout << B << endl;
    IOHDF5.AppendDataSet(file, "/Data/", "test", B);
  }
  cin.ignore();
  WorldComm.BarrierSync(); // Sync procs within each clone.

}

void Simulation::MPITests()
{
  // Run MPI Tests
  TestInverse(IntraComm);
  TestAllGatherCols(IntraComm);
  TestBroadcast(IntraComm);
  TestSendReceive(IntraComm);
  TestSendrecv(IntraComm);
  TestSums(IntraComm);
}
