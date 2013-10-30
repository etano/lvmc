#include "Communication.h"

#ifdef USE_MPI

#if PRECISION==double
MPI_Datatype MPI_RealType = MPI_DOUBLE;
#elif PRECISION==single
MPI_Datatype MPI_RealType = MPI_FLOAT;
#endif

/// Sets this communicator to be that of all the processes (i.e. MPI_WORLD)
void CommunicatorClass::SetWorld()
{
  MPIComm = MPI_COMM_WORLD;
}

int CommunicatorClass::MyProc()
{
  int rank;
  MPI_Comm_rank(MPIComm, &rank);
  return rank;
}

int CommunicatorClass::NumProcs()
{
  int nProcs;
  MPI_Comm_size(MPIComm, &nProcs);
  return nProcs;
}

string CommunicatorClass::MyHost()
{
  int len;
  char hostname[MPI_MAX_PROCESSOR_NAME];
  MPI_Get_processor_name(hostname, &len);
  return hostname;
}

void CommunicatorClass::BarrierSync()
{
  MPI_Barrier(MPIComm);
}

void CommunicatorClass::Split(int color, CommunicatorClass &newComm)
{
  MPI_Comm_split(MPIComm, color, 0, &(newComm.MPIComm));
}

void CommunicatorClass::Subset(Imatrix &ranks, CommunicatorClass &newComm)
{
  MPI_Group myGroup, newGroup;
  MPI_Comm_group (MPIComm, &myGroup);
  MPI_Group_incl(myGroup, ranks.size(), ranks.memptr(), &newGroup);
  MPI_Comm_create(MPIComm, newGroup, &(newComm.MPIComm));
}

/// Sends

int CommunicatorClass::Send(int toProc, Tmatrix &buff)
{
  return MPI_Send(buff.memptr(), buff.size(), MPI_RealType, toProc, 1, MPIComm);
}

int CommunicatorClass::Send(int toProc, Tvector &buff)
{
  return MPI_Send(buff.memptr(), buff.size(), MPI_RealType, toProc, 1, MPIComm);
}

int CommunicatorClass::Send(int toProc, Imatrix &buff)
{
  return MPI_Send(buff.memptr(), buff.size(), MPI_INT, toProc, 1, MPIComm);
}

int CommunicatorClass::Send(int toProc, Ivector &buff)
{
  return MPI_Send(buff.memptr(), buff.size(), MPI_INT, toProc, 1, MPIComm);
}

int CommunicatorClass::Send (int toProc, RealType val)
{
  return MPI_Send(&val, 1, MPI_RealType, toProc, 0, MPIComm);
}

int CommunicatorClass::Send (int toProc, int val)
{
  return MPI_Send(&val, 1, MPI_INT, toProc, 0, MPIComm);
}

/// Receives

int CommunicatorClass::Receive (int fromProc, Tmatrix &buff)
{
  return MPI_Recv(buff.memptr(), buff.size(), MPI_RealType, fromProc, 1, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::Receive (int fromProc, Tvector &buff)
{
  return MPI_Recv(buff.memptr(), buff.size(), MPI_RealType, fromProc, 1, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::Receive (int fromProc, Imatrix &buff)
{
  return MPI_Recv(buff.memptr(), buff.size(), MPI_INT, fromProc, 1, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::Receive (int fromProc, Ivector &buff)
{
  return MPI_Recv(buff.memptr(), buff.size(), MPI_INT, fromProc, 1, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::Receive (int fromProc, RealType &val)
{
  return MPI_Recv(&val, 1, MPI_INT, fromProc, 0, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::Receive (int fromProc, int &val)
{
  return MPI_Recv(&val, 1, MPI_INT, fromProc, 0, MPIComm, MPI_STATUS_IGNORE);
}

/// Sendrecvs

int CommunicatorClass::SendReceive (int fromProc, Tmatrix &fromBuff, int toProc, Tmatrix &toBuff)
{
  return MPI_Sendrecv(fromBuff.memptr(), fromBuff.size(), MPI_RealType, fromProc, 3, toBuff.memptr(), toBuff.size(), MPI_RealType, toProc, 3, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::SendReceive (int fromProc, Tvector &fromBuff, int toProc, Tvector &toBuff)
{
  return MPI_Sendrecv(fromBuff.memptr(), fromBuff.size(), MPI_RealType, fromProc, 3, toBuff.memptr(), toBuff.size(), MPI_RealType, toProc, 3, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::SendReceive (int fromProc, Imatrix &fromBuff, int toProc, Imatrix &toBuff)
{
  return MPI_Sendrecv(fromBuff.memptr(), fromBuff.size(), MPI_INT, fromProc, 3, toBuff.memptr(), toBuff.size(), MPI_INT, toProc, 3, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::SendReceive (int fromProc, Ivector &fromBuff, int toProc, Ivector &toBuff)
{
  return MPI_Sendrecv(fromBuff.memptr(), fromBuff.size(), MPI_INT, fromProc, 3, toBuff.memptr(), toBuff.size(), MPI_INT, toProc, 3, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::SendReceive (int fromProc, RealType &fromBuff, int toProc, RealType &toBuff)
{
  return MPI_Sendrecv(&fromBuff, 1, MPI_RealType, fromProc, 3, &toBuff, 1, MPI_RealType, toProc, 3, MPIComm, MPI_STATUS_IGNORE);
}

int CommunicatorClass::SendReceive (int fromProc, int &fromBuff, int toProc, int &toBuff)
{
  return MPI_Sendrecv(&fromBuff, 1, MPI_INT, fromProc, 3, &toBuff, 1, MPI_INT, toProc, 3, MPIComm, MPI_STATUS_IGNORE);
}

/// AllGathers

int CommunicatorClass::AllGatherCols(Tmatrix &buff)
{
  int nProcs = NumProcs();
  int myProc = MyProc();
  int rows = buff.n_rows;
  int cols = buff.n_cols;
  int displacements[nProcs];
  int receiveCounts[nProcs];
  int sendCount;
  void *sendBuf, *receiveBuf;
  int currCol = 0;
  for (int proc=0; proc<nProcs; proc++) {
    int procCols = cols/nProcs + ((cols%nProcs)>proc);
    displacements[proc] = rows*currCol;
    receiveCounts[proc] = rows*procCols;
    if (proc == myProc) {
      sendBuf = &(buff(0,currCol));
      sendCount = rows*procCols;
    }
    currCol += procCols;
  }
  receiveBuf = buff.memptr();
  return MPI_Allgatherv(sendBuf, sendCount, MPI_DOUBLE, receiveBuf, receiveCounts, displacements, MPI_DOUBLE, MPIComm);
}

#endif






