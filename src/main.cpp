#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
  COMM::Init(argc, argv);

  // Get input file
  string inFile = "";
  if( argc == 2 ) {
    inFile = argv[1];
  } else {
    cout << "Usage: ./test InputFile\n";
    return 1;
  }
  INPUT::settings In;
  In.load(inFile);

  // Build MPI model
  CommunicatorClass WorldComm; // This is the global MPIWORLD communicator.
  CommunicatorClass IntraComm; // This is for commmunication between procs within a clone group.
  CommunicatorClass InterComm; // This is for communication between the rank 0 procs of each clone group.

  int N = WorldComm.NumProcs();
  int procsPerClone = In.procsPerClone;
  assert ((N % procsPerClone) == 0);
  int nClones = N/procsPerClone;
  int myClone = WorldComm.MyProc()/procsPerClone;
  WorldComm.Split(myClone, IntraComm);
  Ivector ranks(nClones);
  for (int clone=0; clone<nClones; clone++)
    ranks(clone) = clone*procsPerClone;
  WorldComm.Subset(ranks, InterComm);
  int myWorldProc = WorldComm.MyProc();
  if (myWorldProc == 0)
    cout <<"# Procs: "<<N<< ", # Clones: "<<nClones<<", Procs/Clones: "<<procsPerClone<<endl;
  cin.ignore();
  WorldComm.BarrierSync(); // Sync procs within each clone.

  // Run MPI Tests
  TestAllGatherCols(IntraComm);
  TestBroadcast(IntraComm);
  TestSendReceive(IntraComm);
  TestSendrecv(IntraComm);
  TestSums(IntraComm);

  COMM::Finalize();

  return 0;
}

