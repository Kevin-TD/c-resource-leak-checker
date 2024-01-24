#ifndef PROGRAM_POINT_H
#define PROGRAM_POINT_H

#include "ProgramRepresentation/DisjointPVAliasSets.h"

// reflects a branch that holds some instructions in the IR. this class manages
// a point's program variables using a set of alias sets. it is effectively a
// wrapper for DisjointPVAliasSets
class ProgramPoint {
  private:
    DisjointPVAliasSets programVariableAliasSets;

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

    // adds variable programVar if it does not already exist in these sets
    void addVariable(ProgramVariable programVar);

    // adds pvas into this set of alias sets. pvas's program variables will merge
    // into one of these sets if it contains a program variable that exists in one
    // of these sets
    void addPVAS(PVAliasSet pvas);

    // finds set A and B from element A and element B (respectively) and merges
    // them together. if A == B or one of the elements is not found in any of these
    // sets, no actions are performed
    void makeAliased(ProgramVariable elementA, ProgramVariable elementB);

    // copies the disjointed sets from programVariableAliasSets into these alias sets
    void
    setProgramVariableAliasSets(DisjointPVAliasSets programVariableAliasSets);

    DisjointPVAliasSets getProgramVariableAliasSets() const;

    // returns a pointer to an alias set based on programVar. if addNewIfNotFound is true,
    // if we do not find the alias set, we will add a new set that consists of only
    // programVar. if addNewIfNotFound is false, NULL is returned
    PVAliasSet *getPVASRef(ProgramVariable programVar, bool addNewIfNotFound);

    // returns a pointer to an alias set if there is a set that contains a program variable with name
    // cleanedName. if addNewIfNotFound is true,
    // if we do not find the alias set, we will add a new set that consists of only
    // programVar. if addNewIfNotFound is false, NULL is returned
    PVAliasSet *getPVASRef(const std::string& cleanedName, bool addNewIfNotFound);

    // returns a reference to the alias set if there is a set that contains a program variable with
    // that value pointer
    PVAliasSet *getPVASRef(Value* value, bool addNewIfNotFound);

    std::string getPointName() const;

    // compares self and another point ref to see if they have the same program
    // variables and methods set
    bool equals(ProgramPoint *programPoint);

    // add the alias sets in programPoint to self. a set may be merged into an
    // existenting set (in which case, methods set is not updated) or is added
    // into this set of sets (in which case, program variables and methods set are
    // added)
    void add(ProgramPoint *programPoint);

    // checks if programVar is in any of these sets
    bool varExists(ProgramVariable programVar);
};

#endif