#include "ProgramRepresentation/DisjointPVAliasSets.h"
#include "Debug.h"

typename std::list<PVAliasSet>::iterator
DisjointPVAliasSets::findIter(ProgramVariable programVar) {
    for (auto it = sets.begin(); it != sets.end(); ++it) {
        if (it->contains(programVar)) {
            return it;
        }
    }

    return sets.end();
}

void DisjointPVAliasSets::merge(
    typename std::list<PVAliasSet>::iterator set1,
    typename std::list<PVAliasSet>::iterator set2) {
    if (set1 != sets.end() && set2 != sets.end()) {
        set1->programVariables.splice(set1->programVariables.end(),
                                      set2->programVariables);
        sets.erase(set2);
    }
}
bool DisjointPVAliasSets::elementIsInAnySet(ProgramVariable programVar) {
    return findIter(programVar) != sets.end();
}

std::list<PVAliasSet> DisjointPVAliasSets::getSets() const {
    return sets;
}

PVAliasSet *DisjointPVAliasSets::getSetRef(ProgramVariable programVar) {
    return getSetRef(programVar.getCleanedName());
}

PVAliasSet *DisjointPVAliasSets::getSetRef(const std::string& cleanedName) {
    for (PVAliasSet &set : sets) {
        if (set.contains(cleanedName)) {
            return &set;
        }
    }

    return NULL;
}

PVAliasSet *DisjointPVAliasSets::getSetRef(Value* val) {
    for (PVAliasSet &set : sets) {
        if (set.contains(val)) {
            return &set;
        }
    }

    return NULL;
}

void DisjointPVAliasSets::unionSets(ProgramVariable elementA,
                                    ProgramVariable elementB) {
    auto set1 = findIter(elementA);
    auto set2 = findIter(elementB);

    if (set1 == set2 || set1 == sets.end() || set2 == sets.end()) {
        return;
    }

    merge(set1, set2);
}
void DisjointPVAliasSets::makeSet(ProgramVariable programVar) {
    if (getSetRef(programVar)) {
        return;
    }

    PVAliasSet newSet;
    newSet.add(programVar);
    sets.push_back(newSet);
}

void DisjointPVAliasSets::addAlias(ProgramVariable element1,
                                   ProgramVariable element2) {

    PVAliasSet* element1Set = this->getSetRef(element1);
    PVAliasSet* element2Set = this->getSetRef(element2);

    // case: both sets exist
    if (element1Set && element2Set) {
        element2Set->changeSetNumbersBy(element1Set->getMaxSetNumber() + 1);
        this->unionSets(element1, element2);
        return;
    }


    // case: 1 of the sets exist
    if (element1Set) {
        element1Set->add(element2);
        return;
    }

    if (element2Set) {
        element2Set->add(element1);
        return;
    }


    // case: neither of the sets exist
    PVAliasSet newSet;
    newSet.programVariables = {element1, element2};
    sets.push_back(newSet);

}

void DisjointPVAliasSets::clear() {
    sets.clear();
}

void DisjointPVAliasSets::mergeSet(PVAliasSet pvas) {

    for (ProgramVariable pv : pvas.getProgramVariables()) {
        auto foundAliasSet = findIter(pv);

        if (foundAliasSet != sets.end()) {
            foundAliasSet->addProgramVariables(pvas.getProgramVariables());
            return;
        }
    }

    sets.push_back(pvas);
}

int DisjointPVAliasSets::size() const {
    return sets.size();
}
