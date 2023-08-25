#include "ProgramRepresentation/ProgramPoint.h"
#include "Debug.h"


ProgramPoint::ProgramPoint(std::string name) {
  this->name = name;
}

ProgramPoint::ProgramPoint() {

}

void ProgramPoint::addAlias(ProgramVariable receiving,
                                       ProgramVariable receiver) {
  for (ProgramVariable& pv : this->programVariables) {
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

void ProgramPoint::fillAlias(std::string varNameCleaned, ProgramVariable variable) {
  for (ProgramVariable& pv : this->programVariables) {
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
      logout("FILLING WITH ALIAS " << variable.getCleanedName() << " FOR HEAD VAR " << pv.getCleanedName() << " RAW NAME " << pv.getRawName())
      pv.addAlias(variable);
      return; 
    }
  }

}

std::list<ProgramVariable> ProgramPoint::getProgramVariables() {
  return this->programVariables;
}

std::set<std::string>
ProgramPoint::findVarAndNamedAliases(std::string cleanedName) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return pv.getNamedAliases(true);
    }

    for (std::string alias : pv.getAllAliases(true)) {
      if (alias == cleanedName) {
        return pv.getNamedAliases(true);
      }
    }
  }

  return std::set<std::string>();
}


std::string ProgramPoint::getName() {
  return this->name; 
}

ProgramVariable ProgramPoint::getProgramVariableByValue(Value* value) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getValue() == value) {
      return pv;
    }

    for (ProgramVariable alias : pv.getPValiases()) {
      if (alias.getValue() == value) {
        return alias;
      }
    }
  }
  return ProgramVariable(value); 
}


ProgramVariable ProgramPoint::getProgramVariableByRawName(std::string rawName) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getRawName() == rawName) {
      return pv;
    }

    for (ProgramVariable alias : pv.getPValiases()) {
      if (alias.getRawName() == rawName) {
        return alias;
      }
    }
  }
  return ProgramVariable();
}

ProgramVariable ProgramPoint::getProgramVariableByCleanedName(std::string cleanedName) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return pv;
    }

    for (ProgramVariable* alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return *alias;
      }
    }
  }
  ProgramVariable newPV = ProgramVariable(cleanedName);
  this->programVariables.push_back(newPV); 
  logout("3.MAKING NEW FOR " << cleanedName)
  return newPV; 
}

ProgramVariable* ProgramPoint::getProgramVariableByCleanedNameRef(std::string cleanedName) {
  for (ProgramVariable& pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return &pv;
    }

    for (ProgramVariable* alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return alias;
      }
    }
  }
  ProgramVariable newPV = ProgramVariable(cleanedName);
  this->addVariable(newPV); 
  logout("2.MAKING NEW FOR " << cleanedName)
  return getProgramVariableByCleanedNameRef(cleanedName);
}

ProgramVariable* ProgramPoint::getOnlyMainProgramVariableByCleanedNameRef(std::string cleanedName) {
  for (ProgramVariable& pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return &pv;
    }

    for (ProgramVariable* alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return &pv;
      }
    }
  }
  ProgramVariable newPV = ProgramVariable(cleanedName);
  this->addVariable(newPV); 
  logout("1.MAKING NEW FOR " << cleanedName)
  return getOnlyMainProgramVariableByCleanedNameRef(cleanedName);
}

ProgramVariable* ProgramPoint::getOnlyMainPVCleanNameGenericIfNotFound(std::string cleanedName) {
  for (ProgramVariable& pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return &pv;
    }

    for (ProgramVariable* alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return &pv;
      }
    }
  }
  return new ProgramVariable(); 
}

ProgramVariable ProgramPoint::getOnlyMainProgramVariableByCleanedName(std::string cleanedName) {
  for (ProgramVariable pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return pv;
    }

    for (ProgramVariable* alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return pv;
      }
    }
  }
  ProgramVariable newPV = ProgramVariable(cleanedName);
  this->addVariable(newPV); 
  return getOnlyMainProgramVariableByCleanedName(cleanedName);
}

bool ProgramPoint::equals(ProgramPoint programPoint) {
  if (this->name != programPoint.getName()) {
    return false; 
  }
  
  for (ProgramVariable pv1 : this->programVariables) {
    std::string cleanedName = pv1.getCleanedName();
    ProgramVariable pv2 = programPoint.getOnlyMainProgramVariableByCleanedName(cleanedName);

    if (!pv1.equals(pv2)) {
      return false; 
    }
  }

  for (ProgramVariable pv2 : programPoint.getProgramVariables()) {
    std::string cleanedName = pv2.getCleanedName();
    ProgramVariable pv1 = this->getOnlyMainProgramVariableByCleanedName(cleanedName);

    if (!pv2.equals(pv1)) {
      return false; 
    }
  }

  return true; 
}

bool ProgramPoint::programVariableExistsByCleanedName(std::string cleanedName) {
  for (ProgramVariable pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return true;
    }

    for (ProgramVariable* alias : pv.generatePVptrAliases()) {
      if (alias->getCleanedName() == cleanedName) {
        return true;
      }
    }
  }
  
  return false;
}

bool ProgramPoint::checkMainHeadExists(std::string cleanedName) {
  for (ProgramVariable pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return true;
    }
  }
  
  return false;
}

void ProgramPoint::setProgramVariables(std::list<ProgramVariable> programVariables) {
  this->programVariables.clear();
  this->programVariables = programVariables; 
}

ProgramPoint ProgramPoint::copy(ProgramPoint programPoint) {
  std::string cloneName = programPoint.getName();
  std::list<ProgramVariable> programVariablesClone(programPoint.getProgramVariables());

  ProgramPoint programPointClone = ProgramPoint(cloneName);

  programPointClone.setProgramVariables(programVariablesClone);
  
  return programPointClone;
}

void ProgramPoint::add(ProgramPoint otherPoint) {
  for (ProgramVariable pv : otherPoint.getProgramVariables()) {
    std::string pvCleanedName = pv.getCleanedName();
    
    if (this->checkMainHeadExists(pvCleanedName)) {

      for (ProgramVariable pvAlias : pv.getPValiases()) {
        this->fillAlias(pvCleanedName, pvAlias);

      }
    } else {
      logout("ADDING VAR " << pvCleanedName)
      logout("raw name " << pv.getRawName())
      this->addVariable(pv);
    }

  }
}