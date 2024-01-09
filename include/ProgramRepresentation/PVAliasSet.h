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

    // returns true iff there is a program var in this set with Value* value.
    // this overload should only be used if you are looking for value that may
    // not exist in the branch you're currently analyzing.
    bool contains(Value* value);

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

    // sets this methods set to the union of this method set and other
    void methodsSetUnion(const MethodsSet other);

    // sets this methods set to the intersection of this method set and other
    void methodsSetIntersection(const MethodsSet other);

    // adds method to set of methods called on this
    void addMethod(std::string method);

    // empties the set of methods called on this
    void clearMethods();

    // void setMethodsSet(MethodsSet methods);

    // generates the set of program variables represented by, if cleanNames is true, their clean name
    // or, if cleanNames if false, their raw name
    std::string toString(bool cleanNames) const;

    // generates the set of methods called as a string.
    std::string getMethodsString() const;

    /* iterates through this list of program variables to see if any of them
    contain an index. the first program variable we find that has an index is
    returned. if no such program variables exist, -1 is returned.

    Explanation of usage:

    Sometimes, we create a new alias set with a program variable that refers to the field of some struct (i.e., some for some struct my_struct { char* x, char* y } and var my_struct M, we may make a new alias set { M.x }. When creating that program variable, we call the ProgramVariable(Value *value, int index) constructor, letting us specify that this variable has a field.

    Problems arise by the fact that when we need to add more program variables to the alias set, they likely won't have a field specified. E.g., we can let some my_struct* p = &M.x, and it would not make sense to copy the index of M to p. There are also IR intermediate variables that complicate this as well.

    So say we have some alias set { M.x, p, %27, %28 }. Of this set, only M.x has an index. We need to be able to access this index somehow, anyhow. (Note: a very important use of getIndex is when we look for annotations that specify an index). Since these are must-aliases, it's safe to grab any index in this set as they should all refer to M.x , and so, we can grab the first one which has an index.

    Note that this would also work if we did something like M a; M b; a.x = b.y. It would be safe to grab the index of either of these variables since their annotation methods specified will be the same, or the lattice of a.x would be lower than a.y. If they aren’t, the annotation verifier will throw an error anyhow.
    */
    int getIndex();

    // returns true iff any of this program variables contains an index, meaning it refers to
    // a struct's field. returns false iff there is no program variable with an index
    bool containsStructFieldVar();

    friend class DisjointedPVAliasSets;
    friend class TestRunner;
};

#endif