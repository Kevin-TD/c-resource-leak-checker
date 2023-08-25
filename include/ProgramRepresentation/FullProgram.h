#ifndef FULL_PROGRAM_H
#define FULL_PROGRAM_H

#include "ProgramRepresentation/ProgramFunction.h"

// reflects a file. this class manages all the functions that make up a program.
class FullProgram {
private:
  std::list<ProgramFunction> programFunctions;

  // refers to file name
  std::string name;

public:
  FullProgram();
  FullProgram(std::string name);

  void addProgramFunction(ProgramFunction programFunction);

  std::list<ProgramFunction> getProgramFunctions();

  ProgramFunction getProgramFunction(std::string name);
  ProgramFunction *getProgramFunctionRef(std::string name);

  std::string getName();
};

#endif