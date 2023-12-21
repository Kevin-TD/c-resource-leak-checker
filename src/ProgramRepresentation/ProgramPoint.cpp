#include "ProgramRepresentation/ProgramPoint.h"
#include "Debug.h"

ProgramPoint::ProgramPoint(std::string pointName) {
    this->pointName = pointName;
}

ProgramPoint::ProgramPoint() {}

ProgramPoint::ProgramPoint(std::string pointName, ProgramPoint *programPoint) {
  this->pointName = pointName;
  this->programVariableAliasSets = programPoint->getProgramVariableAliasSets();
}

void ProgramPoint::logoutProgramPoint(const ProgramPoint &point,
                                      bool logMethods) {
  logout("\n**point name " << point.getPointName());
  for (auto aliasSet : point.getProgramVariableAliasSets().getSets()) {
    logout("> alias set = " << aliasSet.toString(false));

    if (logMethods) {
      logout("--> methods set = " << aliasSet.getMethodsString());
    }
  }
}

void ProgramPoint::logoutProgramPoint(const ProgramPoint *point,
                                      bool logMethods) {
  logout("\n**point name " << point->getPointName());
  for (auto aliasSet : point->getProgramVariableAliasSets().getSets()) {
    logout("> alias set = " << aliasSet.toString(false));

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

void ProgramPoint::addPVAliasSet(PVAliasSet pvas) {
  this->programVariableAliasSets.mergeSet(pvas);
}

DisjointedPVAliasSets ProgramPoint::getProgramVariableAliasSets() const {
  return this->programVariableAliasSets;
}

std::string ProgramPoint::getPointName() const { return this->pointName; }

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

  }

  if (addNewIfNotFound) {
    ProgramVariable newPV = ProgramVariable(cleanedName);
    ProgramVariable newPV = ProgramVariable(value);
    this->addVariable(newPV);
    return this->programVariableAliasSets.sets.back();
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

    return true;
}

bool ProgramPoint::varExists(ProgramVariable programVar) {
  return programVariableAliasSets.elementIsInAnySet(programVar);
}

void ProgramPoint::setProgramVariableAliasSets(
    DisjointedPVAliasSets programVariableAliasSets) {
  this->programVariableAliasSets.clear();
  this->programVariableAliasSets = programVariableAliasSets;
}

void ProgramPoint::add(ProgramPoint *programPoint) {
  for (PVAliasSet pvas :
       programPoint->getProgramVariableAliasSets().getSets()) {
    this->programVariableAliasSets.mergeSet(pvas);
  }
}
