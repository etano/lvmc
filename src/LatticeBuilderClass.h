#ifndef LatticeBuilderClass_H
#define LatticeBuilderClass_H

#include <iterator>     // std::ostream_iterator
#include <algorithm>    // std::copy

#include "Utils/IO/InputClass.h"
#include "Utils/IO/IOClass.h"

struct Lattice
{
  Tmatrix T_ij, J_ij;
  Imatrix nn_ij;
  Tvector U;
  Tvector V;

  Lattice(int ns, int nc)
  {
    T_ij.resize(ns,nc);
    J_ij.resize(ns,nc);
    nn_ij.resize(ns,nc);
    U.resize(ns);
    V.resize(ns);
  };
};

class LatticeBuilder
{
private:

  struct Neighbor
  {
    vector<int> dL; // Lattice grid Site to hop to
    int ss; // subLattice Site to hop to
    void Report()
    {
      cout<<" s_ij="<<ss<<"  dX,dY=";
      copy(dL.begin(), dL.end(), ostream_iterator<int>(cout, " "));
      cout<<endl;
    };
  };

  struct Subsite
  {
    int id;
    int globalId;
    vector<RealType> T_ij;
    vector<RealType> J_ij;
    vector<int> nn_ij;
    vector<Neighbor> nn;
    RealType U, V;

    void addNeighbor(Neighbor& n) { nn.push_back(n); };
    int nNN() { return nn.size(); };
    bool checkSize() { return (nn.size()==T_ij.size()); };

    void Report()
    {
      cout<<"  on-site U: "<<U<<endl;
      cout<<"  external Potential V: "<<V<<endl;
      cout<<"  N_Neighbors: "<<nn.size()<<endl;
      for(int i(0);i<nn.size();i++)
        cout<<"   Neighbor("<<i<<"): ("<<T_ij[i]<<", "<<J_ij[i]<<", "<<nn_ij[i]<<")  ";
      cout<<endl;
      for(int i(0);i<nn.size();i++)
        nn[i].Report();
    };

  };

  struct Site
  {
    vector<Subsite> SubSites;
    int id;

    void add(Subsite& ss) { SubSites.push_back(ss); };
    int nSubSites() { return SubSites.size(); };

    void Report()
    {
      cout<<"N_SubSites: "<<SubSites.size()<<endl;
      for(int i(0);i<SubSites.size();i++)
      {
        cout<<" SubSite("<<i<<"):"<<endl;
        SubSites[i].Report();
      }
    };
  };

protected:
  string name;
  IOClass& out;
public:
  // Constructor
  LatticeBuilder(Input &in, IOClass &tmpOut)
    : out(tmpOut)
  {
    out.CreateGroup("Lattice");
    in = in.getChild("Lattice");
    Init(in);
  }

  // Properties
  vector<int> TileMat;
  int nD, nSites, nLatticeSites, nSubSites;
  vector<Site> Sites;

  // Functions
  void Init(Input &in);
  int GetNeighborId(int id, Neighbor &nn);
  Tmatrix* BuildTMatrix();
  Tmatrix* BuildJMatrix();
  Lattice* BuildLattice();
  void Report();

};

#endif
