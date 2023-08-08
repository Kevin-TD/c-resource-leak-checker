#ifndef PROGRAM_VARIABLES_HANDLER_H
#define PROGRAM_VARIABLES_HANDLER_H

#include "ProgramVariable.h"

class ProgramVariablesHandler {
private:
  std::list<ProgramVariable> programVariables;

public:
  void addAlias(ProgramVariable receiving, ProgramVariable receiver);

  std::list<ProgramVariable> getProgramVariables();

  std::set<std::string> findVarAndNamedAliases(std::string cleanedName);

  std::list<ProgramVariable *>
  findProgramVarAndNamedAliases(std::string cleanedName);
};

#endif