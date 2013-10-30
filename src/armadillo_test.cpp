#include <iostream>
#include "config.h"
#include "Communication/Communication.h"
#include "MPITests.h"

using namespace std;

int main(int argc, char** argv)
{
  COMM::Init(argc, argv);

  int myWorldProc = COMM::WorldProc();

  // Build MPI model
  CommunicatorClass WorldComm; // This is the global MPIWORLD communicator.
  CommunicatorClass IntraComm; // This is for commmunication between procs within a clone group.
  CommunicatorClass InterComm; // This is for communication between the rank 0 procs of each clone group.

  int N = WorldComm.NumProcs();
  int procsPerClone = 4;
  assert ((N % procsPerClone) == 0);
  int nClones = N/procsPerClone;
  int myClone = WorldComm.MyProc()/procsPerClone;
  WorldComm.Split(myClone, IntraComm);
  Ivector ranks(nClones);
  for (int clone=0; clone<nClones; clone++)
    ranks(clone) = clone*procsPerClone;
  WorldComm.Subset(ranks, InterComm);
  if (myWorldProc == 0)
    cout <<"# Procs: "<<N<< ", # Clones: "<<nClones<<", Procs/Clones: "<<procsPerClone<<endl;
  cin.ignore();
  WorldComm.BarrierSync(); // Sync procs within each clone.

  // Run MPI Tests
  TestSendReceive(IntraComm);
  TestSendrecv(IntraComm);
  TestAllGatherCols(IntraComm);

  COMM::Finalize();

  return 0;
}

