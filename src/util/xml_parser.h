#ifndef XMLPARSER
#define XMLPARSER

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>

#include "config.h"

namespace prop_tree = boost::property_tree;
using namespace std;

namespace xmlparser
{
  void read(const string& file_name, prop_tree::ptree& pt)
  {
    std::ifstream ifs(file_name.c_str(), std::ifstream::in);
    read_xml(ifs, pt, prop_tree::xml_parser::trim_whitespace);
    ifs.close();
  };

  void write(const string& file_name, const prop_tree::ptree& pt)
  {
    std::ofstream ofs(file_name.c_str(), std::ostream::out);
    write_xml(ofs, pt, boost::property_tree::xml_writer_settings<char>(' ', 2));
    ofs.close();
  };

};
#endif
