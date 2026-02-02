#include "ProgramRepresentation/DisjointPVAliasSets.h"
#include "ProgramRepresentation/ProgramFunction.h"
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

PVAliasSet *DisjointPVAliasSets::findMatchingSet(std::list<ProgramVariable> vars) {
    PVAliasSet *ret = NULL;
    for(auto p = vars.begin(); p != vars.end(); ++p) {
        ret = this->getSetRef(*p);
        if(ret) {
            return ret;
        }
    }
    return NULL;
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

PVAliasSet *DisjointPVAliasSets::getSetRefID(int id) {
    for (PVAliasSet &set : sets) {
        if (set.getID() == id) {
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
void DisjointPVAliasSets::makeSet(ProgramVariable programVar, int newID) {
    if (getSetRef(programVar)) {
        return;
    }

    PVAliasSet newSet;
    newSet.setID(newID);
    newSet.add(programVar);
    sets.push_back(newSet);
}

// First argument is "new" second is "old" for example item = %1 will be addAlias(item, %1)
void DisjointPVAliasSets::addAlias(ProgramVariable element1,
                                   ProgramVariable element2, ProgramFunction *pf) {

    PVAliasSet* element1Set = this->getSetRef(element1);
    PVAliasSet* element2Set = this->getSetRef(element2);

    // case: both sets exist
    // Earlier, this joined two sets, this causes an error under the following example:
    // Register 0 and 1 point to different resources.
    // Register 0 is stored in item
    // Register 1 is stored in item
    // Now these aliases will be joined into the same set despite pointing to different resources
    // Registers are SSA, we can reason that the only time this case will happen is when
    // we are storing something into memory. In that case we will remove the variable from the old set
    // and store it in the new set
    if (element1Set && element2Set) {
        element1Set->moveOut(element1);
        element2Set->add(element1);
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
    int newID = pf->getNewID();
    newSet.setID(newID);

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
