#ifndef DISJOINED_PV_ALIAS_SETS_H
#define DISJOINED_PV_ALIAS_SETS_H

#include "ProgramRepresentation/PVAliasSet.h"

// manages a set of alias sets
class DisjointedPVAliasSets {
private:
  std::list<PVAliasSet> sets;

  // returns a reference to the alias set programVar belongs to. if it's not
  // found, .end() is returned.
  typename std::list<PVAliasSet>::iterator findIter(ProgramVariable programVar);

  // returns a reference to the alias set where there is a program variables
  // with raw name rawName. if it's not found, .end() is returned.
  typename std::list<PVAliasSet>::iterator
  findIterByRawName(const std::string &rawName);

  // returns a reference to the alias set where there is a program variables
  // with cleaned name cleanedName. if it's not found, .end() is returned.
  typename std::list<PVAliasSet>::iterator
  findIterByCleanedName(const std::string &cleanedName);

  // puts the elements of set2 into set1 in constant time. set2 is also deleteed
  // from our set of sets.
  void merge(typename std::list<PVAliasSet>::iterator set1,
             typename std::list<PVAliasSet>::iterator set2);

public:
  // returns true iff programVar is in any of the disjointed sets
  bool elementIsInAnySet(ProgramVariable programVar);

  // returns true iff there is a program variable in any of the sets
  // with cleaned name cleanedName
  bool varIsInAnySetByCleanedName(const std::string &cleanedName);

  // returns true iff there is a program variable in any of the sets
  // with raw name rawName
  bool varIsInAnySetByRawName(const std::string &rawName);

  // returns a copy of every disjointed set
  std::list<PVAliasSet> getSets() const;

  // returns the alias set belonging to programVar. an empty set is returned if
  // it's not found.
  PVAliasSet getSet(ProgramVariable programVar);

  // returns the alias set that has a program variable with cleaned name
  // cleanedName. an empty set is returned if it's not found.
  PVAliasSet getSet(const std::string &cleanedName);

  // returns a reference to the alias set that has a program variable with
  // cleaned name cleanedName. an empty set is returned if it's not found.
  PVAliasSet *getSetRef(const std::string &cleanedName);

  // unions sets A and B of elementA and elementB. if A == B, or elementA or
  // elementB are not actually elements of any set, the union is not performed.
  void unionSets(ProgramVariable elementA, ProgramVariable elementB);

  // creates a new disjoint set if it's already not an element in any of the
  // existing sets.
  void makeSet(ProgramVariable programVar);

  // aliases element1 and element2 by putting them into the same set. if one of
  // the elements already belongs in an existing set, both elements will be in
  // that set. otherwise, they will be put into a new set.
  void addAlias(ProgramVariable element1, ProgramVariable element2);

  // removes all sets
  void clear();

  int getNumberOfSets();

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