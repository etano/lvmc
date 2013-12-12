#include "InputFile.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

// Loads settings structure from the specified XML file
void Input::load(const std::string &filename)
{
  // Create an empty property tree object
  using boost::property_tree::ptree;
  ptree pt;

  // Load the XML file into the property tree. If reading fails
  // (cannot open file, parse error), an exception is thrown.
  read_xml(filename, pt);

  // Inputs
  outputPrefix = pt.get<std::string>("Input.IO.OutputPrefix");
  procsPerGroup = pt.get<int>("Input.Parallel.ProcsPerGroup", 1);
}

