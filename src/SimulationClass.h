#ifndef SimulationClass_H
#define SimulationClass_H

#include <iostream>
#include "Utils/config.h"
#include "Utils/Communication/Communication.h"
#include "Utils/IO/InputFile.h"
#include "Utils/IO/IO.h"
#include "Utils/RNG/RNG.h"

class Simulation
{
public:
  // Constructor
#if USE_MPI
  Simulation() : rng((int)time(0)*(COMM::WorldProc()+1)) {}
#else
  Simulation() : rng((int)time(0)) {}
#endif

  // Simulation Random Number Generator
  RNG rng;

  // Inputs
  Input In;
  void ReadInput(string inFile);

  // MPI setup
  CommunicatorClass WorldComm; // This is the global MPIWORLD communicator.
  CommunicatorClass InterComm; // This is for communication between the rank 0 procs of each walker group.
  CommunicatorClass IntraComm; // This is for commmunication between procs within a walker group.
  void BuildMPIModel();

  // Algorithm
  void Run();

  // Tests
  void Tests();
  void HDF5Tests();
  void MPITests();
  void ReturnSync();
  void TestInverse(CommunicatorClass &MyComm);
  void TestAllGatherCols(CommunicatorClass &MyComm);
  void TestBroadcast(CommunicatorClass &MyComm);
  void TestSendReceive(CommunicatorClass &MyComm);
  void TestSendrecv(CommunicatorClass &MyComm);
  void TestSums(CommunicatorClass &MyComm);

};

#endif
