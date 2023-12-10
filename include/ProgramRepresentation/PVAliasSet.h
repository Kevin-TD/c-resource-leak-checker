#ifndef PV_ALIAS_SET_H
#define PV_ALIAS_SET_H

#include "ProgramRepresentation/MethodsSet.h"
#include "ProgramRepresentation/ProgramVariable.h"
#include "Utils.h"

// holds a set of program variables and methods called on them.
// each program variable in the set is considered an alias set.
class PVAliasSet {
private:
  std::list<ProgramVariable> programVariables;

  // holds the methods called on the set
  MethodsSet methods;

public:
  PVAliasSet();

  // returns true iff programVar is in the set by checking its raw name
  bool contains(ProgramVariable programVar);

  // returns true iff there exists a program variable in our set with raw name
  // equal to rawName
  bool containsByRawName(const std::string &rawName);

  // returns true iff there exists a program variable in our set with clean name
  // equal to cleanedName
  bool containsByCleanedName(const std::string &cleanedName);

  // adds programVar to our set of program variables. programVar is checked to
  // see if it already exists in our set of program variables and it's not added
  // if it already does.
  void add(ProgramVariable programVar);

  // adds array of program variables to our set of program variables.
  // each variable is checked to see if already exists in our list of program
  // variables.
  void addProgramVariables(std::list<ProgramVariable> programVariables);

  std::list<ProgramVariable> getProgramVariables();

  MethodsSet getMethodsSet() const;
  MethodsSet *getMethodsSetRef();

  void setMethodsSet(MethodsSet methods);

  // generates the set of program variables represented by their cleaned name as
  // a string
  std::string getVarsStringUsingCleanedNames() const;

  // generates the set of program variables represented by their raw name as a
  // string
  std::string getVarsStringUsingRawNames() const;

  // generates the set of methods called as a string.
  std::string getMethodsString() const;

  // iterates through our list of program variables to see if any of them
  // contain an index. the first program variable we find that has an index is
  // returned. if no such program variables exist, -1 is returned.
  int getIndex();

  // returns true iff any of our program variables contains an index. returns
  // false iff there is no program variable with an index
  bool hasIndex();

  friend class DisjointedPVAliasSets;
};

#endif