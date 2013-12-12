#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <string>
#include "../config.h"

class Input
{
public:
  std::string outputPrefix; // prefix for all generated output files
  int procsPerGroup; // # of processors per clone
  void load(const std::string &filename);
};

#endif
