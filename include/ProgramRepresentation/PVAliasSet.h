#ifndef PV_ALIAS_SET_H
#define PV_ALIAS_SET_H

#include "ProgramRepresentation/MethodsSet.h"
#include "ProgramRepresentation/ProgramVariable.h"
#include "Utils.h"

// holds a set of program variables and methods called on them.
// each program variable in the set is a must-alias of all the other variables in that set.
class PVAliasSet {
  private:
    std::list<ProgramVariable> programVariables;

    // holds the methods called on the set
    MethodsSet methods;

  public:
    PVAliasSet();

    // returns true iff programVar is in the set by checking its raw name
    bool contains(ProgramVariable programVar);

    // return true iff there is a program var in this set with cleaned name cleanedName.
    // it is preferable to use the overload that specifies a ProgramVariable to avoid
    // collisions between local (%) and global (@) LLVM identifiers. see
    // LLVM docs: https://llvm.org/docs/LangRef.html#identifiers
    bool contains(const std::string& cleanedName);


    // adds programVar to this set of program variables. programVar is checked to
    // see if it already exists in this set of program variables and it's not added
    // if it already does.
    void add(ProgramVariable programVar);

    // adds array of program variables to this set of program variables.
    // each variable is checked to see if already exists in this list of program
    // variables.
    void addProgramVariables(std::list<ProgramVariable> programVariables);

    std::list<ProgramVariable> getProgramVariables();

    MethodsSet getMethodsSet() const;
    MethodsSet *getMethodsSetRef();

    void setMethodsSet(MethodsSet methods);

    // generates the set of program variables represented by, if cleanNames is true, their clean name
    // or, if cleanNames if false, their raw name
    std::string toString(bool cleanNames) const;

    // generates the set of methods called as a string.
    std::string getMethodsString() const;

    // iterates through this list of program variables to see if any of them
    // contain an index. the first program variable we find that has an index is
    // returned. if no such program variables exist, -1 is returned.
    int getIndex();

    // returns true iff any of this program variables contains an index, meaning it refers to
    // a struct's field. returns false iff there is no program variable with an index
    bool containsStructFieldVar();

    friend class DisjointedPVAliasSets;
};

#endif