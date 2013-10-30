#include "MPITests.h"

void ReturnSync() {
  if (COMM::WorldProc() == 0)
    cout << "Hit return to continue..." << endl;
  cin.ignore();
  COMM::BarrierSync();
}

void TestAllGatherCols(CommunicatorClass &MyComm) {
  int myProc = MyComm.MyProc();
  int nProcs = MyComm.NumProcs();
  int nCols = 2*nProcs+2;
  int nRows = 2;
  Tmatrix A = myProc*arma::ones<Tmatrix>(nRows,nCols);
  MyComm.AllGatherCols(A);
  if (myProc==A(0,int(nCols*float(myProc)/float(nProcs)+1.)))
    cout << myProc << " passes." << endl;
  ReturnSync();
}

void TestSendReceive(CommunicatorClass &MyComm) {
  int myProc = MyComm.MyProc();
  if (myProc == 0) {
    Imatrix value = arma::ones<Imatrix>(2,2);
    MyComm.Send(1, value);
  } else if (myProc == 1) {
    Imatrix value = arma::zeros<Imatrix>(2,2);
    Imatrix tmpValue = arma::ones<Imatrix>(2,2);
    MyComm.Receive(0, value);
    if (arma::accu(value-tmpValue)==0) {
      cout << "0 passes." << endl;
      cout << "1 passes." << endl;
    }
  }
  ReturnSync();
}

void TestSendrecv(CommunicatorClass &MyComm) {
  int nProcs = MyComm.NumProcs();
  int myProc = MyComm.MyProc();
  int sendProc = (myProc+1) % nProcs;
  int recvProc = ((myProc-1) + nProcs) % nProcs;
  Imatrix value1 = myProc*arma::ones<Imatrix>(2,2);
  Imatrix value2 = recvProc*arma::ones<Imatrix>(2,2);
  MyComm.SendReceive(sendProc, value1, recvProc, value1);
  if (arma::accu(value1-value2)==0)
    cout << myProc << " passes." << endl;
  ReturnSync();
}
