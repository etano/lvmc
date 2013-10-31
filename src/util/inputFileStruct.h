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
    enum{silent=0, essential, chatty, debug};

    prop_tree::ptree pt;
    std::string infile_name;
    std::string outfile_name;

// General controls
    int verbosity;
    std::vector<int> p_grid;

// for optimization 
    bool opt;
    RealType ftol,xtol;

// for GAP function
    std::vector<RealType> f_parameters;
    RealType f_exp;

// for Configurations
    std::string conf_file;
    int n_test, n_train;

    control_block(const string& iname): infile_name(iname), outfile_name("default_out.xml"), verbosity(0), 
                     opt(false), ftol(0.01), xtol(0.01), f_exp(2.0), conf_file("test.xyz"), n_test(100), n_train(100)
    {
      p_grid.resize(2,1);
    };

    void read()
    {
      xmlparser::read(infile_name, pt);

      verbosity=pt.get<int>("control.verbosity");
      std::cout<< "Verbosity: " << verbosity <<std::endl;

      p_grid[0]=pt.get<int>("control.parallel.nc");
      p_grid[1]=pt.get<int>("control.parallel.nr");
      if(verbosity>1)
        std::cout<<p_grid[0]<<" "<<p_grid[1]<<std::endl;

      f_exp=pt.get<RealType>("control.function.exp");
      if(verbosity>1)
        std::cout<<f_exp<<std::endl;

      string pvec_str(pt.get<string>("control.function.params"));
      std::istringstream iss(pvec_str);
      std::copy(std::istream_iterator<RealType>(iss), std::istream_iterator<RealType>(), std::back_inserter(f_parameters));
      if(verbosity>1)
      {
        std::copy(f_parameters.begin(), f_parameters.end(),
        std::ostream_iterator<RealType>(std::cout, "  "));
        std::cout<<std::endl;
      }

      ftol=pt.get<RealType>("control.opt.ftol");
      xtol=pt.get<RealType>("control.opt.xtol");
      opt=pt.get<bool>("control.opt.on");
      if( (verbosity>1) and opt )
        std::cout<<xtol<<" "<<ftol<<std::endl;

      n_train=pt.get<int>("control.configurations.train");
      n_test=pt.get<int>("control.configurations.test");
      conf_file=pt.get<string>("control.configurations.file");
      if(verbosity>1)
        std::cout<< conf_file <<" "<<n_test<<" "<<n_train<<std::endl;

      outfile_name=pt.get<string>("control.output");
      if(verbosity>1)
        std::cout<< outfile_name <<std::endl;

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
      ar & cb.p_grid;
      ar & cb.verbosity;
      ar & cb.opt;
      ar & cb.ftol;
      ar & cb.xtol;
      ar & cb.f_parameters;
      ar & cb.f_exp;
      ar & cb.conf_file;
      ar & cb.n_test;
      ar & cb.n_train;      
    }
  } // namespace serialization
} // namespace boost

#endif
