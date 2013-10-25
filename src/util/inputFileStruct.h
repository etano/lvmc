#ifndef INFILESTRUCT
#define INFILESTRUCT

#include <string>
#include <sstream>
#include <algorithm>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_serialization.hpp>
#include <boost/foreach.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "config.h"
#include "util/xml_parser.h"

using namespace std;


template<class T>
struct parameterset
{
  std::string name;
  std::vector<T> param;
  void print()
  {
    std::cout<<name<<endl;
    std::copy(param.begin(), param.end(),
        std::ostream_iterator<T>(std::cout, "  "));
    std::cout<<endl;
  };
};

template<class T>
struct parameter
{
  std::string name;
  T param;
  void print()
  {
    std::cout<<name<<endl;
    std::cout<<param<<endl;
  };
};

struct control_block
{ 
  public:
    enum{silent=0, essential, chatty, debug};
  
    prop_tree::ptree pt;
    std::string infile_name;
    std::string outfile_name;
    parameterset<RealType> pvec;
    parameter<bool> p;
    int verbosity;

    
    void read(const string& iname)
    {
      infile_name=iname;
      xmlparser::read(iname, pt);
      
      p.name="boolean";
      verbosity=pt.get<int>("control.verbosity");
      std::cout<< "Verbosity: " << verbosity <<std::endl;

      p.name="boolean";
      p.param=pt.get<bool>("control.p");
      if(verbosity>1)
      p.print();
      
      pvec.name="real vector";
      string pvec_str(pt.get<string>("control.pset"));
      std::istringstream iss(pvec_str);
      std::copy(std::istream_iterator<RealType>(iss), std::istream_iterator<RealType>(), std::back_inserter(pvec.param));
      if(verbosity>1)
        pvec.print();
    };
    
    void write(const string& oname)
    {
      outfile_name=oname;
      xmlparser::write(oname, pt);
    };
};

namespace boost {
  namespace serialization {
    
    template<class Archive, class T>
    void serialize(Archive & ar, parameterset<T> & pms, const unsigned int version)
    {
            ar & pms.name;
            ar & pms.param;
    }
    
    template<class Archive, class T>
    void serialize(Archive & ar, parameter<T> & pm, const unsigned int version)
    {
            ar & pm.name;
            ar & pm.param;
    }
    
    template<class Archive>
    void serialize(Archive & ar, control_block & cb, const unsigned int version)
    {
            ar & cb.pt;
            ar & cb.infile_name;
            ar & cb.outfile_name;
            ar & cb.pvec;
            ar & cb.p;
            ar & cb.verbosity;
    }
  } // namespace serialization
} // namespace boost

#endif