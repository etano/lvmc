#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "../config.h"

namespace COMM
{
#ifdef USE_MPI // Parallel version
  inline void Init (int argc, char **argv)
  {
    MPI_Init(&argc, &argv);
    int proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
  }

  inline void Finalize()
  {
    MPI_Finalize();
  }

  inline int WorldProc()
  {
    int proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    return (proc);
  }

  inline void BarrierSync()
  {
    MPI_Barrier(MPI_COMM_WORLD);
  }

#else // Serial version
  inline void Init (int argc, char **argv) {}
  inline void Finalize () {}
  inline int WorldProc() {return (0);}
#endif
}


class CommStatusClass
{
public:
  int Source;
  int Tag;
  int Error;
  int Length;
};


class CommunicatorClass
{
public:
#ifdef USE_MPI // Parallel version
  MPI_Comm MPIComm;

  void SetWorld(); // Sets this communicator to be that of all the processes (i.e. MPI_WORLD)
  int MyProc();
  int NumProcs();
  string MyHost();
  void BarrierSync();
  void Split(int color, CommunicatorClass &newComm);
  void Subset(Imatrix &ranks, CommunicatorClass &newComm);

  // Sends
  int Send (int toProc, Tmatrix &buff);
  int Send (int toProc, Tvector &buff);
  int Send (int toProc, Imatrix &buff);
  int Send (int toProc, Ivector &buff);
  int Send (int toProc, RealType val);
  int Send (int toProc, int val);

  // Receives
  int Receive (int fromProc, Tmatrix &buff);
  int Receive (int fromProc, Tvector &buff);
  int Receive (int fromProc, Imatrix &buff);
  int Receive (int fromProc, Ivector &buff);
  int Receive (int fromProc, RealType &val);
  int Receive (int fromProc, int &val);

  // Sendrecvs
  int SendReceive (int fromProc, Tmatrix &fromBuff, int toProc, Tmatrix &toBuff);
  int SendReceive (int fromProc, Tvector &fromBuff, int toProc, Tvector &toBuff);
  int SendReceive (int fromProc, Imatrix &fromBuff, int toProc, Imatrix &toBuff);
  int SendReceive (int fromProc, Ivector &fromBuff, int toProc, Ivector &toBuff);
  int SendReceive (int fromProc, RealType &fromBuff, int toProc, RealType &toBuff);
  int SendReceive (int fromProc, int &fromBuff, int toProc, int &toBuff);

  // Broadcasts

  // Sums

  // Gathers

  // AllGathers
  int AllGatherCols (Tmatrix &buff);

  CommunicatorClass()
  {
    SetWorld();
  }

#else   // Serial version
  inline void SetWorld(){}
  inline int MyProc() {return 0;}
  inline int NumProcs() {return 1;}
  inline string MyHost() {return "0";}
  inline void BarrierSync() {}
  inline void Split(int color, CommunicatorClass &newComm) {}
  inline void Subset(Imatrix ranks, CommunicatorClass &newComm)
  {
    if (ranks.size() != 1) {
      cerr << "Serial verion of code does not support nontrivial subsets. Exiting.\n";
      abort();
    }
  }

#endif
};

#endif

