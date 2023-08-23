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
  return getProgramVariableByCleanedNameRef(cleanedName);
}

bool ProgramPoint::equals(ProgramPoint programPoint) {
  if (this->name != programPoint.getName()) {
    return false; 
  }
  
  for (ProgramVariable pv1 : this->programVariables) {
    MethodsSet methods1 = pv1.getMethodsSet(); 
    Value* value1 = pv1.getValue(); 

    for (ProgramVariable pv2 : programPoint.getProgramVariables()) {
      Value* value2 = pv2.getValue(); 
      MethodsSet methods2 = pv2.getMethodsSet(); 

      if (value1 == value2 && methods1.getMethods() != methods2.getMethods()) {
        return false; 
      }
    }
  }

  return true; 


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