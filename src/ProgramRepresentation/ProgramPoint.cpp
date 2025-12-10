#include "ProgramRepresentation/ProgramPoint.h"
#include "Debug.h"

ProgramPoint::ProgramPoint(int pointLine) {
    this->pointLine = pointLine;
}

ProgramPoint::ProgramPoint() {}

ProgramPoint::ProgramPoint(int pointLine, ProgramPoint *programPoint) {
    this->pointLine = pointLine;
    this->programVariableAliasSets = programPoint->getProgramVariableAliasSets();
}

void ProgramPoint::logoutProgramPoint(const ProgramPoint &point,
                                      bool logMethods) {
    logout("\n**point name " << point.getPointLine());
    for (auto aliasSet : point.getProgramVariableAliasSets().getSets()) {
        logout("> alias set = " << aliasSet.toString(false, false));

        if (logMethods) {
            logout("--> methods set = " << aliasSet.getMethodsString());
        }
    }
}

void ProgramPoint::logoutProgramPoint(const ProgramPoint *point,
                                      bool logMethods) {
    logout("\n**point name " << point->getPointLine());
    for (auto aliasSet : point->getProgramVariableAliasSets().getSets()) {
        logout("> alias set = " << aliasSet.toString(false, false));

        if (logMethods) {
            logout("--> methods set = " << aliasSet.getMethodsString());
        }
    }
}

void ProgramPoint::addAlias(ProgramVariable element1,
                            ProgramVariable element2) {
    this->programVariableAliasSets.addAlias(element1, element2);
}

void ProgramPoint::makeAliased(ProgramVariable elementA,
                               ProgramVariable elementB) {
    this->programVariableAliasSets.unionSets(elementA, elementB);
}

void ProgramPoint::addVariable(ProgramVariable programVar) {
    this->programVariableAliasSets.makeSet(programVar);
}

void ProgramPoint::addPVAS(PVAliasSet pvas) {
    this->programVariableAliasSets.mergeSet(pvas);
}

DisjointPVAliasSets ProgramPoint::getProgramVariableAliasSets() const {
    return this->programVariableAliasSets;
}

int ProgramPoint::getPointLine() const {
    return this->pointLine;
}

PVAliasSet *ProgramPoint::getPVASRef(ProgramVariable programVar,
                                     bool addNewIfNotFound) {
    PVAliasSet *pvas = this->programVariableAliasSets.getSetRef(programVar);

    if (pvas) {
        return pvas;
    }

    if (addNewIfNotFound) {
        this->addVariable(programVar);
        return &this->programVariableAliasSets.sets.back();
    }

    return NULL;
}

PVAliasSet *ProgramPoint::getPVASRef(const std::string& cleanedName, bool addNewIfNotFound) {
    PVAliasSet *pvas = this->programVariableAliasSets.getSetRef(cleanedName);

    if (pvas) {
        return pvas;
    }

    if (addNewIfNotFound) {
        ProgramVariable newPV = ProgramVariable(cleanedName);
        this->addVariable(newPV);
        return &this->programVariableAliasSets.sets.back();
    }

    return NULL;
}

PVAliasSet *ProgramPoint::getPVASRef(Value* value,
                                     bool addNewIfNotFound) {

    PVAliasSet *pvas = this->programVariableAliasSets.getSetRef(value);

    if (pvas) {
        return pvas;
    }

    if (addNewIfNotFound) {
        ProgramVariable newPV = ProgramVariable(value);
        this->addVariable(newPV);
        return &this->programVariableAliasSets.sets.back();
    }

    return NULL;
}

bool ProgramPoint::equals(ProgramPoint *programPoint) {
    for (PVAliasSet set : this->programVariableAliasSets.sets) {
        PVAliasSet *pointAliasSet;

        for (ProgramVariable pv : set.getProgramVariables()) {
            pointAliasSet = programPoint->getPVASRef(pv, false);
            if (pointAliasSet) {
                break;
            }
        }

        if (!pointAliasSet) {
            return false;
        }

        if (!set.getMethodsSet().equals(pointAliasSet->getMethodsSet())) {
            return false;
        }
    }

    for (PVAliasSet pointSet :
            programPoint->getProgramVariableAliasSets().getSets()) {
        PVAliasSet *selfAliasSet;
        for (ProgramVariable pointPV : pointSet.getProgramVariables()) {
            selfAliasSet = this->getPVASRef(pointPV, false);

            if (selfAliasSet) {
                break;
            }
        }

        if (!selfAliasSet) {
            return false;
        }

        if (!pointSet.getMethodsSet().equals(selfAliasSet->getMethodsSet())) {
            return false;
        }
    }

    return true;
}

bool ProgramPoint::varExists(ProgramVariable programVar) {
    return programVariableAliasSets.elementIsInAnySet(programVar);
}

void ProgramPoint::setProgramVariableAliasSets(
    DisjointPVAliasSets programVariableAliasSets) {
    this->programVariableAliasSets.clear();
    this->programVariableAliasSets = programVariableAliasSets;
}

void ProgramPoint::add(ProgramPoint *programPoint) {
    for (PVAliasSet pvas :
            programPoint->getProgramVariableAliasSets().getSets()) {
        this->programVariableAliasSets.mergeSet(pvas);
    }
}

void ProgramPoint::unalias(PVAliasSet* pvas, const std::string& cleanedNameOfPVToUnalias, ProgramVariable pvCallInst, ProgramVariable callInstAlias) {
    for (ProgramVariable& pv : pvas->getProgramVariables()) {
        if (pv.equalsCleanedName(cleanedNameOfPVToUnalias)) {
            if (pv.getFieldIndex() != -1) {
                PVAliasSet grabbedSet = pvas->moveOut(pv.getSetNumber());
                grabbedSet.add(pvCallInst);

                if (pvas->getProgramVariables().size() > 0) {
                    ProgramVariable pvToMove = pvas->moveOut(callInstAlias);
                    if (pvToMove.getCleanedName() != "") {
                        grabbedSet.add(pvToMove);
                    }
                }

                addPVAS(grabbedSet);
                break;
            } else {
                ProgramVariable pvToMove = pvas->moveOut(pv);
                PVAliasSet newSet;
                newSet.add(pvToMove);
                newSet.add(pvCallInst);
                addPVAS(newSet);
                break;
            }
        }
    }
}

void ProgramPoint::addSuccessor(ProgramPoint *p) {
    successors.push_back(p);
}

std::list<ProgramPoint *> ProgramPoint::getSuccessors() {
    return successors;
}

void ProgramPoint::unalias(PVAliasSet* pvas, const std::string& cleanedNameOfPVToUnalias, ProgramVariable argumentVar) {
    for (ProgramVariable& pv : pvas->getProgramVariables()) {
        if (pv.equalsCleanedName(cleanedNameOfPVToUnalias)) {
            // TODO: replace all instances of
            // below where where check if
            // `field != -1` to use `ProgramVariable::containsStructFieldVar` instead as
            // checking this is the purpose that method serves
            if (pv.getFieldIndex() != -1) {
                PVAliasSet grabbedSet = pvas->moveOut(pv.getSetNumber());

                ProgramVariable argVarPV = pvas->moveOut(argumentVar);
                if (argVarPV.getCleanedName() != "") {
                    grabbedSet.add(argVarPV);
                }

                addPVAS(grabbedSet);
                break;
            } else {
                PVAliasSet newSet;

                ProgramVariable pvToMove = pvas->moveOut(pv);
                newSet.add(pvToMove);

                ProgramVariable argVarPV = pvas->moveOut(argumentVar);
                newSet.add(argVarPV);

                addPVAS(newSet);
                break;
            }
        }
    }
}


void ProgramPoint::remove(ProgramVariable pv) {
    PVAliasSet *pvas = this->programVariableAliasSets.getSetRef(pv);
    if(pvas == NULL) {
        return;
    }
    pvas->moveOut(pv);
}
