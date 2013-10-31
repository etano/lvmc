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
  int itWorked = 0;
  if (myProc==A(0,int(nCols*float(myProc)/float(nProcs)+1.)))
    itWorked = 1;
  int sum = 0;
  MyComm.Sum(0, itWorked, sum);
  if (myProc == 0 && sum == MyComm.NumProcs())
    cout << "AllGatherCols test ... passed." << endl;

  ReturnSync();
}

void TestBroadcast(CommunicatorClass &MyComm) {
  int myProc = MyComm.MyProc();

  // int test
  int myValue = myProc;
  MyComm.Broadcast(0, myValue);
  if (myProc == 1 && myValue == 0)
    cout << "Broadcast int test ... passed." << endl;

  // Imatrix test
  Imatrix myMat = myProc*arma::ones<Imatrix>(2,2);
  MyComm.Broadcast(0, myMat);
  Imatrix knownMat = 0*arma::ones<Imatrix>(2,2);
  if (myProc == 1 && arma::accu(myMat-knownMat)==0)
    cout << "Broadcast Imatrix test ... passed." << endl;

  ReturnSync();
}

void TestSendReceive(CommunicatorClass &MyComm) {
  int myProc = MyComm.MyProc();

  // int test
  if (myProc == 0) {
    int value = 17;
    MyComm.Send(1, value);
  } else if (myProc == 1) {
    int value = 0;
    int tmpValue = 17;
    MyComm.Receive(0, value);
    if (arma::accu(value-tmpValue)==0) {
      cout << "Send/Receive int test ... passed." << endl;
    }
  }

  // Imatrix test
  if (myProc == 0) {
    Imatrix value = arma::ones<Imatrix>(2,2);
    MyComm.Send(1, value);
  } else if (myProc == 1) {
    Imatrix value = arma::zeros<Imatrix>(2,2);
    Imatrix tmpValue = arma::ones<Imatrix>(2,2);
    MyComm.Receive(0, value);
    if (arma::accu(value-tmpValue)==0) {
      cout << "Send/Receive Imatrix test ... passed." << endl;
    }
  }

  ReturnSync();
}

void TestSendrecv(CommunicatorClass &MyComm) {
  int nProcs = MyComm.NumProcs();
  int myProc = MyComm.MyProc();
  int sendProc = (myProc+1) % nProcs;
  int recvProc = ((myProc-1) + nProcs) % nProcs;

  // int test
  int val1 = myProc;
  int val2 = recvProc;
  MyComm.SendReceive(sendProc, val1, recvProc, val1);
  if (myProc == 0 && val1==val2)
    cout << "SendReceive int test ... passed." << endl;

  // Imatrix test
  Imatrix mat1 = myProc*arma::ones<Imatrix>(2,2);
  Imatrix mat2 = recvProc*arma::ones<Imatrix>(2,2);
  MyComm.SendReceive(sendProc, mat1, recvProc, mat1);
  if (myProc == 0 && arma::accu(mat1-mat2)==0)
    cout << "SendReceive Imatrix test ... passed." << endl;

  ReturnSync();
}

void TestSums(CommunicatorClass &MyComm) {
  int myProc = MyComm.MyProc();

  // int test
  int myValue = 1;
  int sum = 0;
  MyComm.Sum(0, myValue, sum);
  if (myProc == 0 && sum == MyComm.NumProcs())
    cout << "Sum int test ... passed." << endl;

  // Imatrix test
  Imatrix myMat = arma::ones<Imatrix>(2,2);
  Imatrix mySum = arma::zeros<Imatrix>(2,2);
  MyComm.Sum(0, myMat, mySum);
  Imatrix knownSum = MyComm.NumProcs()*arma::ones<Imatrix>(2,2);
  if (myProc == 0 && arma::accu(mySum-knownSum)==0)
    cout << "Sum Imatrix test ... passed." << endl;
  ReturnSync();
}
