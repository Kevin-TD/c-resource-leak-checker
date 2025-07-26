#ifndef DISJOINED_PV_ALIAS_SETS_H
#define DISJOINED_PV_ALIAS_SETS_H

#include "ProgramRepresentation/PVAliasSet.h"

// manages a disjoint set (https://en.wikipedia.org/wiki/Disjoint-set_data_structure)
// of program variable must-aliases.
// each program variable is an element of no more than 1 set.
class DisjointPVAliasSets {
  private:
    std::list<PVAliasSet> sets;

    // returns a reference to the alias set programVar belongs to. if it's not
    // found, .end() is returned.
    typename std::list<PVAliasSet>::iterator findIter(ProgramVariable programVar);

    // puts the elements of set2 into set1 in constant time. set2 is also deleted
    // from our set of sets. if set1 or set2 is null, no merging or deletion occurs.
    void merge(typename std::list<PVAliasSet>::iterator set1,
               typename std::list<PVAliasSet>::iterator set2);

  public:
    // returns true iff programVar is in any of the disjointed sets
    bool elementIsInAnySet(ProgramVariable programVar);

    // returns a copy of every disjointed set
    std::list<PVAliasSet> getSets() const;

    // returns a reference to the alias set that has programVar.
    // NULL is returned if it's not found.
    PVAliasSet *getSetRef(ProgramVariable programVar);

    // returns a reference to the alias set that has a program variable
    // with name cleanedName. NULL is return if it's not found.
    // it is preferable to use the overload that specifies a ProgramVariable to avoid
    // collisions between local (%) and global (@) LLVM identifiers. see
    // LLVM docs: https://llvm.org/docs/LangRef.html#identifiers
    PVAliasSet *getSetRef(const std::string& cleanedName);

    // returns a reference to the alias set that has a program variable
    // with value val. NULL is return if it's not found.
    PVAliasSet *getSetRef(Value* val);

    // unions the sets that contain either elementA or elementB. if elementA == elementB, or
    // elementA or elementB are not actually elements of any set, the union is not performed.
    void unionSets(ProgramVariable elementA, ProgramVariable elementB);

    // creates a new disjoint set containing programVar. if programVar is
    // an existing member of any of the sets, no set is created.
    void makeSet(ProgramVariable programVar);

    // aliases element1 and element2 by putting them into the same set. if one of
    // the elements already belongs in an existing set, both elements will be in
    // that set. otherwise, they will be put into a new set.
    // additionally, if both sets (set1 and set2) exist, the set numbers
    // of set2 will be changed by set1's maximum set number plus 1.
    void addAlias(ProgramVariable element1, ProgramVariable element2);

    // removes all sets
    void clear();

    // returns the number of disjointed sets
    int size() const;

    // adds pvas by either merging it into one of our sets or adding it to our set
    // of sets. the program variables in pvas are merged if it has a program
    // variable that equals any of our own program variables; in this case, its
    // method set is not merged, only the variables. if none of the program
    // variables equals any of our own program variables, the set itself (program
    // variables and method set) is added to our set of sets (pvas is copied).
    void mergeSet(PVAliasSet pvas);

    friend class ProgramPoint;
};

#endif