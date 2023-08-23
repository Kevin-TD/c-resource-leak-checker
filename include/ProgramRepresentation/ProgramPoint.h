#ifndef PROGRAM_POINT_H
#define PROGRAM_POINT_H

#include "ProgramRepresentation/ProgramVariable.h"

class ProgramPoint {
private:
  std::list<ProgramVariable> programVariables;

  // the name is same as the branch name that shows up in the IR
  std::string name;

public:
  ProgramPoint(); 
  ProgramPoint(std::string name); 

  void addAlias(ProgramVariable receiving, ProgramVariable receiver);

  void addVariable(ProgramVariable programVar);

  void setProgramVariables(std::list<ProgramVariable> programVariables); 

  std::list<ProgramVariable> getProgramVariables();

  ProgramVariable getProgramVariableByValue(Value* value);
  ProgramVariable getProgramVariableByRawName(std::string rawName);

  ProgramVariable getProgramVariableByCleanedName(std::string cleanedName);
  ProgramVariable* getProgramVariableByCleanedNameRef(std::string cleanedName);

  std::set<std::string> findVarAndNamedAliases(std::string cleanedName);

  std::string getName(); 

  bool equals(ProgramPoint programPoint); 

  static ProgramPoint copy(ProgramPoint programPoint);
};

#endif