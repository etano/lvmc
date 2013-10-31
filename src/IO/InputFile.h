#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace INPUT{
  struct settings
  {
    std::string outputPrefix; // prefix for all generated output files
    int procsPerClone; // # of processors per clone
    void load(const std::string &filename);
  };

  // Loads settings structure from the specified XML file
  void settings::load(const std::string &filename)
  {
      // Create an empty property tree object
      using boost::property_tree::ptree;
      ptree pt;

      // Load the XML file into the property tree. If reading fails
      // (cannot open file, parse error), an exception is thrown.
      read_xml(filename, pt);

      // Prefix for all generated output files
      outputPrefix = pt.get<std::string>("Input.IO.OutputPrefix");

      // # of processors per clone, default 1
      procsPerClone = pt.get("Input.Parallel.ProcsPerClone", 1);
  }

}

#endif
