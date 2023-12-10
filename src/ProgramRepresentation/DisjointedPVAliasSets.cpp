#include "ProgramRepresentation/DisjointedPVAliasSets.h"
#include "Debug.h"

typename std::list<PVAliasSet>::iterator
DisjointedPVAliasSets::findIter(ProgramVariable programVar) {
  for (auto it = sets.begin(); it != sets.end(); ++it) {
    if (it->contains(programVar)) {
      return it;
    }
  }

  return sets.end();
}

typename std::list<PVAliasSet>::iterator
DisjointedPVAliasSets::findIterByRawName(const std::string &rawName) {
  for (auto it = sets.begin(); it != sets.end(); ++it) {
    if (it->containsByRawName(rawName)) {
      return it;
    }
  }

  return sets.end();
}

typename std::list<PVAliasSet>::iterator
DisjointedPVAliasSets::findIterByCleanedName(const std::string &cleanedName) {
  for (auto it = sets.begin(); it != sets.end(); ++it) {
    if (it->containsByCleanedName(cleanedName)) {
      return it;
    }
  }

  return sets.end();
}

void DisjointedPVAliasSets::merge(
    typename std::list<PVAliasSet>::iterator set1,
    typename std::list<PVAliasSet>::iterator set2) {
  if (set1 != sets.end() && set2 != sets.end()) {
    set1->programVariables.splice(set1->programVariables.end(),
                                  set2->programVariables);
    sets.erase(set2);
  }
}
bool DisjointedPVAliasSets::elementIsInAnySet(ProgramVariable programVar) {
  return findIter(programVar) != sets.end();
}

bool DisjointedPVAliasSets::varIsInAnySetByRawName(const std::string &rawName) {
  return findIterByRawName(rawName) != sets.end();
}

bool DisjointedPVAliasSets::varIsInAnySetByCleanedName(
    const std::string &cleanedName) {
  return findIterByCleanedName(cleanedName) != sets.end();
}

std::list<PVAliasSet> DisjointedPVAliasSets::getSets() const { return sets; }

PVAliasSet DisjointedPVAliasSets::getSet(ProgramVariable programVar) {
  for (PVAliasSet set : sets) {
    if (set.contains(programVar)) {
      return set;
    }
  }

  return PVAliasSet();
}

PVAliasSet DisjointedPVAliasSets::getSet(const std::string &cleanedName) {
  for (PVAliasSet set : sets) {
    if (set.containsByCleanedName(cleanedName)) {
      return set;
    }
  }

  return PVAliasSet();
}

PVAliasSet *DisjointedPVAliasSets::getSetRef(const std::string &cleanedName) {
  for (PVAliasSet &set : sets) {
    if (set.containsByCleanedName(cleanedName)) {
      return &set;
    }
  }

  return NULL;
}

void DisjointedPVAliasSets::unionSets(ProgramVariable elementA,
                                      ProgramVariable elementB) {
  auto set1 = findIter(elementA);
  auto set2 = findIter(elementB);

  if (set1 == set2 || set1 == sets.end() || set2 == sets.end()) {
    return;
  }

  merge(set1, set2);
}
void DisjointedPVAliasSets::makeSet(ProgramVariable programVar) {
  PVAliasSet newSet;
  newSet.add(programVar);
  sets.push_back(newSet);
}

void DisjointedPVAliasSets::addAlias(ProgramVariable element1,
                                     ProgramVariable element2) {
  for (auto it = sets.begin(); it != sets.end(); ++it) {
    if (it->contains(element1)) {
      it->add(element2);
      return;
    }

    if (it->contains(element2)) {
      it->add(element1);
      return;
    }
  }

  PVAliasSet newSet;
  newSet.programVariables = {element1, element2};
  sets.push_back(newSet);
}

void DisjointedPVAliasSets::clear() { sets.clear(); }

void DisjointedPVAliasSets::mergeSet(PVAliasSet pvas) {

  for (ProgramVariable pv : pvas.getProgramVariables()) {
    auto foundAliasSet = findIterByCleanedName(pv.getCleanedName());

    if (foundAliasSet != sets.end()) {
      foundAliasSet->addProgramVariables(pvas.getProgramVariables());
      return;
    }
  }

  sets.push_back(pvas);
}

int DisjointedPVAliasSets::getNumberOfSets() { return sets.size(); }