#include "ProgramRepresentation/ProgramPoint.h"
#include "Debug.h"

ProgramPoint::ProgramPoint(std::string pointName) {
  this->pointName = pointName;
}

ProgramPoint::ProgramPoint() {}

ProgramPoint::ProgramPoint(std::string pointName, ProgramPoint *programPoint) {
  this->pointName = pointName;
  this->programVariables = programPoint->getProgramVariables();
}

void ProgramPoint::addAlias(ProgramVariable receiving,
                            ProgramVariable receiver) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getRawName() == receiving.getRawName()) {
      pv.addAlias(receiver);
      return;
    } else if (pv.getRawName() == receiver.getRawName()) {
      pv.addAlias(receiving);
      return;
    }

    for (std::string alias : pv.getAllAliases(false)) {
      if (alias == receiving.getRawName()) {
        pv.addAlias(receiver);
        return;
      } else if (alias == receiver.getRawName()) {
        pv.addAlias(receiving);
        return;
      }
    }
  }

  receiver.addAlias(receiving);
  this->addVariable(receiver);
}

void ProgramPoint::addVariable(ProgramVariable programVar) {
  this->programVariables.push_back(programVar);
}

void ProgramPoint::fillAlias(std::string varNameCleaned,
                             ProgramVariable variable) {
  for (ProgramVariable &pv : this->programVariables) {
    if (varNameCleaned != pv.getCleanedName()) {
      continue;
    }

    bool add = true;

    for (std::string alias : pv.getAllAliases(true)) {
      if (alias == variable.getCleanedName()) {
        add = false;
      }
    }

    if (add) {
      pv.addAlias(variable);
      return;
    }
  }
}

std::list<ProgramVariable> ProgramPoint::getProgramVariables() {
  return this->programVariables;
}

std::string ProgramPoint::getPointName() { return this->pointName; }

ProgramVariable *ProgramPoint::getPVRef(std::string cleanedName,
                                        bool addNewIfNotFound) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return &pv;
    }

    for (ProgramVariable *alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return &pv;
      }
    }
  }
  if (addNewIfNotFound) {
    ProgramVariable newPV = ProgramVariable(cleanedName);
    this->addVariable(newPV);
    return &this->programVariables.back();
  }

  return NULL;
}

ProgramVariable ProgramPoint::getPV(std::string cleanedName,
                                    bool addNewIfNotFound) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return pv;
    }

    for (ProgramVariable *alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return pv;
      }
    }
  }

  if (addNewIfNotFound) {
    ProgramVariable newPV = ProgramVariable(cleanedName);
    this->addVariable(newPV);
    return this->programVariables.back();
  }

  errs() << "Error at getPV: Program var not found and new program var not "
            "added for var '"
         << cleanedName << "'\n";
  std::exit(EXIT_FAILURE);
}

bool ProgramPoint::equals(ProgramPoint programPoint) {
  if (this->pointName != programPoint.getPointName()) {
    return false;
  }

  for (ProgramVariable pv1 : this->programVariables) {
    std::string cleanedName = pv1.getCleanedName();
    ProgramVariable pv2 = programPoint.getPV(cleanedName, false);

    if (!pv1.equals(pv2)) {
      return false;
    }
  }

  for (ProgramVariable pv2 : programPoint.getProgramVariables()) {
    std::string cleanedName = pv2.getCleanedName();
    ProgramVariable pv1 = this->getPV(cleanedName, false);

    if (!pv2.equals(pv1)) {
      return false;
    }
  }

  return true;
}

bool ProgramPoint::equals(ProgramPoint *programPoint) {
  for (ProgramVariable pv1 : this->programVariables) {
    std::string cleanedName = pv1.getCleanedName();
    ProgramVariable *pv2 = programPoint->getPVRef(cleanedName, false);

    if (!pv2 || !pv1.equals(*pv2)) {
      return false;
    }
  }

  for (ProgramVariable pv2 : programPoint->getProgramVariables()) {
    std::string cleanedName = pv2.getCleanedName();
    ProgramVariable *pv1 = this->getPVRef(cleanedName, false);

    if (!pv1 || !pv2.equals(*pv1)) {
      return false;
    }
  }

  return true;
}

bool ProgramPoint::varExists(std::string cleanedName) {
  for (ProgramVariable pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return true;
    }
  }

  return false;
}

void ProgramPoint::setProgramVariables(
    std::list<ProgramVariable> programVariables) {
  this->programVariables.clear();
  this->programVariables = programVariables;
}

void ProgramPoint::setProgramVariables(ProgramPoint *programPoint) {
  this->programVariables.clear();
  this->programVariables = programPoint->getProgramVariables();
}

void ProgramPoint::setProgramVariables(ProgramPoint programPoint) {
  this->programVariables.clear();
  this->programVariables = programPoint.getProgramVariables();
}

void ProgramPoint::add(ProgramPoint *otherPoint) {
  for (ProgramVariable pv : otherPoint->getProgramVariables()) {
    std::string pvCleanedName = pv.getCleanedName();

    if (this->varExists(pvCleanedName)) {

      for (ProgramVariable pvAlias : pv.getPValiases()) {
        this->fillAlias(pvCleanedName, pvAlias);
      }
    } else {
      logout("ADDING VAR " << pvCleanedName)
          logout("raw name " << pv.getRawName()) this->addVariable(pv);
    }
  }
}