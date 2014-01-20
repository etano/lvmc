#include "LatticeBuilderClass.h"

void LatticeBuilder::Init(Input &in)
{

  istringstream iss(in.getAttribute<string>("dims"));
  copy(istream_iterator<int>(iss), istream_iterator<int>(), back_inserter(TileMat));

  nD = TileMat.size();
  int id = 0;
  Site unitCell;
  vector<Input> siteList = in.getChild("Site").getChildList();
  for (int i=0; i<siteList.size(); ++i) {
    Subsite ss;
    ss.id = id;
    id += 1;
    ss.V = siteList[i].getAttribute<RealType>("V");
    ss.U = siteList[i].getAttribute<RealType>("U");

    istringstream hss(siteList[i].getAttribute<string>("J") );
    copy(istream_iterator<RealType>(hss), istream_iterator<RealType>(), back_inserter(ss.J_ij));

    istringstream iss(siteList[i].getAttribute<string>("T") );
    copy(istream_iterator<RealType>(iss), istream_iterator<RealType>(), back_inserter(ss.T_ij));

    vector<int> siteData;
    istringstream jss(siteList[i].getText("Topology"));
    copy(istream_iterator<int>(jss), istream_iterator<int>(), back_inserter(siteData));
    int nn = siteData.size()/(nD+1.);

    for(int i=0; i<nn; i++) {
      Neighbor n;
      n.dL.resize(nD);
      n.ss = siteData[i*(nD+1)];
      for(int j=0; j<nD; j++)
        n.dL[j] = siteData[1+j + i*(nD+1)];
      ss.addNeighbor(n);
    }
    unitCell.add(ss);
  }
  unitCell.id = 0;

  nLatticeSites = 1;
  for(int i=0; i<nD; i++)
    nLatticeSites *= TileMat[i];
  Sites.resize(nLatticeSites, unitCell);

  nSubSites = unitCell.nSubSites();
  nSites = nLatticeSites*nSubSites;

  for(int i=0;i<nLatticeSites;i++)
  {
    Sites[i].id=i;
    for(int j=0;j<nSubSites;j++)
    {
      Sites[i].SubSites[j].globalId=i*nSubSites+j;
      Sites[i].SubSites[j].nn_ij.resize(Sites[i].SubSites[j].T_ij.size());
      for(int k=0;k<Sites[i].SubSites[j].T_ij.size();k++)
        Sites[i].SubSites[j].nn_ij[k] = GetNeighborId(i, Sites[i].SubSites[j].nn[k])*nSubSites + Sites[i].SubSites[j].nn[k].ss;
    }
  }

};

int LatticeBuilder::GetNeighborId(int id, Neighbor &nn)
{
  if (nD==2) {
    // handles grids layed out like a square.
    int lx=id%TileMat[0];
    int ly=id/TileMat[0];
    int lxp = lx + nn.dL[0];
    while (lxp<0)
      lxp+=TileMat[0];
    while (lxp>=TileMat[0])
      lxp-=TileMat[0];
    int lyp = ly + nn.dL[1];
    while (lyp<0)
      lyp+=TileMat[1];
    while (lyp>=TileMat[1])
      lyp-=TileMat[1];
    return lxp + TileMat[0]*lyp;
  } else if (nD==1) {
    //     handles grids layed out like a square.
    int lx=id;
    int lxp = lx + nn.dL[0];
    while (lxp<0)
      lxp+=TileMat[0];
    while (lxp>=TileMat[0])
      lxp-=TileMat[0];
    return lxp;
  } else {
    cout<<"Only 1D and 2D coded. Your tiling is "<<nD<<"-D"<<endl;
    return -1;
  }
}

void LatticeBuilder::Report()
{
  cout<<"N_Sites: "<<Sites.size()<<endl;
  Sites[0].Report();
};

Tmatrix* LatticeBuilder::BuildTMatrix()
{
  Tmatrix* tmat = new Tmatrix(nSites,nSites);
  (*tmat) *= 0;
  for(int i=0; i<nLatticeSites; i++)
    for(int j=0; j<nSubSites; j++) {
      int id = Sites[i].SubSites[j].globalId;
      int nConnections = Sites[i].SubSites[j].nn_ij.size();
      for(int k=0; k<nConnections; k++)
        (*tmat)(id,Sites[i].SubSites[j].nn_ij[k]) = Sites[i].SubSites[j].T_ij[k];
    }
  return tmat;
}

Tmatrix* LatticeBuilder::BuildJMatrix()
{
  Tmatrix* jmat = new Tmatrix(nSites,nSites);
  (*jmat) *= 0;
  for(int i=0; i<nLatticeSites; i++)
    for(int j=0; j<nSubSites; j++) {
      int id = Sites[i].SubSites[j].globalId;
      int nConnections = Sites[i].SubSites[j].nn_ij.size();
      for(int k=0; k<nConnections; k++)
        (*jmat)(id,Sites[i].SubSites[j].nn_ij[k]) = Sites[i].SubSites[j].J_ij[k];
    }
  return jmat;
}

Lattice* LatticeBuilder::BuildLattice()
{
  int nConnections = Sites[0].SubSites[0].nn_ij.size();
  Lattice* lat = new Lattice(nSites, nConnections);
  for(int i=0; i<nLatticeSites; i++)
    for(int j=0; j<nSubSites; j++) {
      int id=Sites[i].SubSites[j].globalId;
      lat->U[id] = Sites[i].SubSites[j].U;
      lat->V[id] = Sites[i].SubSites[j].V;
      for(int k=0; k<nConnections; k++) {
        lat->nn_ij(id,k) = Sites[i].SubSites[j].nn_ij[k];
        lat->T_ij(id,k) = Sites[i].SubSites[j].T_ij[k];
        lat->J_ij(id,k) = Sites[i].SubSites[j].J_ij[k];
      }
    }
  return lat;
}

