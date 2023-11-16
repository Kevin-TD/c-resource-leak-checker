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
  std::string addedAliasRawName;

  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getRawName() == receiving.getRawName()) {
      pv.addAlias(receiver);
      addedAliasRawName = receiver.getRawName();
      break;
    } else if (pv.getRawName() == receiver.getRawName()) {
      pv.addAlias(receiving);
      addedAliasRawName = receiving.getRawName();
      break;
    }

    for (std::string alias : pv.getAllAliases(false)) {
      if (alias == receiving.getRawName()) {
        pv.addAlias(receiver);
        addedAliasRawName = receiver.getRawName();
        break;
      } else if (alias == receiver.getRawName()) {
        pv.addAlias(receiving);
        addedAliasRawName = receiving.getRawName();
        break;
      }
    }
  }

  // if we add an alias that is actually one of the program variables,
  // we remove it from the list of program variables.
  /*
  the following example explains why this was added:

  let's say we have some struct My_struct with 2 fields (0 and 1).
  M is an instance of that struct, and M_ptr is a pointer to M.

  in the IR, we see there are allocate instructions for M and M_ptr;
  they allocate memory for My_struct and My_struct* respectively. at this point
  we do not actually know they are aliased yet, so we destructure these structs
  and insert each field as a program variable. so we'll have our list of program
  variables = {M.0, M.1, M_ptr.0, M_ptr.1}.

  it is only when we see a store instruction (store %M to %M_ptr)
  that we find out that they are aliases. Thus, we should alias
  M.0 with M_ptr.0 and M.1 with M_ptr.1. when we do this,
  our list of program variables looks like this:
  {
    M.0 -> {M_ptr.0},
    M.1 -> {M_ptr.1},
    M_ptr.0,
    M_ptr.1
  }

  you can see that M_ptr.0 and M_ptr.1 repeat twice, being both
  related to another variable and being a variable itself. We
  resolve this by removing the M_ptr.0 and M_ptr.1
  variables and keeping the aliases, so it'll look like this instead:
  {
    M.0 -> {M_ptr.0},
    M.1 -> {M_ptr.1}
  }

  */
  if (addedAliasRawName != "") {
    auto it = this->programVariables.begin();
    while (it != this->programVariables.end()) {
      if (it->getRawName() == addedAliasRawName) {
        it = this->programVariables.erase(it);
      } else {
        it++;
      }
    }
    return;
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
      logout("ADDING VAR " << pvCleanedName);
      logout("raw name " << pv.getRawName());
      this->addVariable(pv);
    }
  }
}