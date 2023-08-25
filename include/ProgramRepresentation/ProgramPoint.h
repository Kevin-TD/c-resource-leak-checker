#ifndef PROGRAM_POINT_H
#define PROGRAM_POINT_H

#include "ProgramRepresentation/ProgramVariable.h"

// reflects a branch that holds some instructions in the IR. this class manages
// a point's program variables
class ProgramPoint {
private:
  std::list<ProgramVariable> programVariables;

  // the name is same as the branch name that shows up in the IR
  std::string name;

  // finds the PV based off a cleaned name and adds variable as an alias to that
  // PV
  void fillAlias(std::string varNameCleaned, ProgramVariable variable);

  // searches to see if there is a "main" program variable with cleanedName,
  // "main" meaning that we are not checking some PV's alias' name
  bool varExists(std::string cleanedName);

public:
  ProgramPoint();
  ProgramPoint(std::string name);
  ProgramPoint(std::string name, ProgramPoint *programPoint);

  void addAlias(ProgramVariable receiving, ProgramVariable receiver);

  void addVariable(ProgramVariable programVar);

  void setProgramVariables(std::list<ProgramVariable> programVariables);
  void setProgramVariables(ProgramPoint *programPoint);
  void setProgramVariables(ProgramPoint programPoint);

  std::list<ProgramVariable> getProgramVariables();

  // returns a pointer to program variable based on the cleadName. if
  // addNewIfNotFound is true, if we do not find the variable, we will add a
  // program variable that just has the cleanedName to our program variables. if
  // addNewIfNotFound is false, an empty ProgramVariable is returned.
  ProgramVariable *getPVRef(std::string cleanedName, bool addNewIfNotFound);

  // returns a program variable based on the cleadName. if addNewIfNotFound is
  // true, if we do not find the variable, we will add a program variable that
  // just has the cleanedName to our program variables. if addNewIfNotFound is
  // false, an empty ProgramVariable is returned.
  ProgramVariable getPV(std::string cleanedName, bool addNewIfNotFound);

  std::string getName();

  // compares self and another point to see if they have the same name and
  // program variables. returns true iff they are equivalent
  bool equals(ProgramPoint programPoint);

  // compares self and another point ref to see if they have the same name and
  // program variables. returns true iff they are equivalent
  bool equals(ProgramPoint *programPoint);

  // merges self and another point only if facts from otherPoint are not in
  // self. no facts are replaced, e.g., if PV with name "x" is in self and in
  // otherPoint, self's fact about "x" will be replaced with otherPoint's fact
  // about "x". if "x" is not in self but is in otherPoint, "x" will be added to
  // self. aliases as well are added, which is crucical to ensuring accurate
  // alias reasoning
  void add(ProgramPoint otherPoint);
};

#endif