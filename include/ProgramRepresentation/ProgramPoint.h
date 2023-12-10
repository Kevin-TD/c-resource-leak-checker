#ifndef PROGRAM_POINT_H
#define PROGRAM_POINT_H

#include "ProgramRepresentation/DisjointedPVAliasSets.h"

// reflects a branch that holds some instructions in the IR. this class manages
// a point's program variables using a set of alias sets. it is effectively a
// wrapper for DisjointedPVAliasSets
class ProgramPoint {
private:
  DisjointedPVAliasSets programVariableAliasSets;

  // the name is same as the branch name that shows up in the IR
  std::string pointName;

public:
  // debugging function that lists variables and methods (methods logged if
  // logMethods is true) of a program point
  static void logoutProgramPoint(const ProgramPoint &point, bool logMethods);

  // debugging function that lists variables and methods (methods logged if
  // logMethods is true) of a program point
  static void logoutProgramPoint(const ProgramPoint *point, bool logMethods);

  ProgramPoint();
  ProgramPoint(std::string pointName);

  // copies the alias sets of programPoint into a new instance
  ProgramPoint(std::string pointName, ProgramPoint *programPoint);

  // aliases element1 and element2 by putting them into the same set
  void addAlias(ProgramVariable element1, ProgramVariable element2);

  // adds variable programVar if it does not already exist in any of our sets
  void addVariable(ProgramVariable programVar);

  // adds pvas into our set of alias sets. pvas's program variables will merge
  // into one of our sets if it contains a program variable that exists in one
  // of our sets
  void addPVAliasSet(PVAliasSet pvas);

  // finds set A and B from element A and element B (respectively) and merges
  // them together. if A == B or one of the elements is not found in any of our
  // sets, the aliasing is not performed.
  void makeAliased(ProgramVariable elementA, ProgramVariable elementB);

  void
  setProgramVariableAliasSets(DisjointedPVAliasSets programVariableAliasSets);

  // copies the alias sets from programPoint into our own alias sets
  void setProgramVariableAliasSets(ProgramPoint *programPoint);

  // copies the alias sets from programPoint into our own alias sets
  void setProgramVariableAliasSets(ProgramPoint programPoint);

  DisjointedPVAliasSets getProgramVariableAliasSets() const;

  // returns a pointer to an alias set based on the cleanedName. if
  // addNewIfNotFound is true, if we do not find the alias set, we will add a
  // new that consists of a single element, a program variable with name
  // cleanedName. if addNewIfNotFound is false, NULL is returned
  PVAliasSet *getPVASRef(const std::string &cleanedName, bool addNewIfNotFound);

  // returns a pointer to an alias set based on the cleanedName. if
  // addNewIfNotFound is true, if we do not find the alias set, we will add a
  // new that consists of a single element, a program variable with name
  // cleanedName. if addNewIfNotFound is false, the program fails and exits.
  PVAliasSet getPVAS(std::string cleanedName, bool addNewIfNotFound);

  std::string getPointName() const;

  // compares self and another point ref to see if they have the same program
  // variables and methods set
  bool equals(ProgramPoint *programPoint);

  // merges self and another point ref only if facts from otherPoint are not in
  // self. no facts are replaced, e.g., if PV with name "x" is in self and in
  // otherPoint, self's fact about "x" will be replaced with otherPoint's fact
  // about "x". if "x" is not in self but is in otherPoint, "x" will be added to
  // self. aliases as well are added, which is crucial to ensuring accurate
  // alias reasoning
  // NOTE: this only updates aliasing info... should change name accordingly ...

  // add the alias sets in programPoint to self. a set may be merged into an
  // existenting set (in which case, methods set is not updated) or is added
  // into our set of sets (in which case, program variables and methods set are
  // added)
  void add(ProgramPoint *programPoint);

  // checks if there is a program variable with name cleanedName in our set
  bool varExists(const std::string &cleanedName);
};

#endif