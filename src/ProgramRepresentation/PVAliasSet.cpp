#include "ProgramRepresentation/PVAliasSet.h"

PVAliasSet::PVAliasSet() {}

bool PVAliasSet::contains(ProgramVariable programVar) {
  for (ProgramVariable pv : programVariables) {
    if (pv.getRawName() == programVar.getRawName()) {
      return true;
    }
  }

  return false;
}

bool PVAliasSet::containsByRawName(const std::string &rawName) {
  for (ProgramVariable pv : programVariables) {
    if (pv.getRawName() == rawName) {
      return true;
    }
  }

  return false;
}

bool PVAliasSet::containsByCleanedName(const std::string &rawName) {
  for (ProgramVariable pv : programVariables) {
    if (pv.getCleanedName() == rawName) {
      return true;
    }
  }

  return false;
}

void PVAliasSet::add(ProgramVariable programVar) {
  if (containsByCleanedName(programVar.getCleanedName())) {
    return;
  }

  programVariables.push_back(programVar);
}

void PVAliasSet::addProgramVariables(
    std::list<ProgramVariable> programVariables) {
  for (ProgramVariable pv : programVariables) {
    add(pv);
  }
}

std::list<ProgramVariable> PVAliasSet::getProgramVariables() {
  return programVariables;
}

MethodsSet PVAliasSet::getMethodsSet() const { return methods; }

MethodsSet *PVAliasSet::getMethodsSetRef() { return &methods; }

void PVAliasSet::setMethodsSet(MethodsSet methods) {
  this->methods.clearMethods();
  this->methods = methods;
}

std::string PVAliasSet::getVarsStringUsingCleanedNames() const {
  std::string result = "{";
  int pvSize = programVariables.size();

  int iterator = 0;
  for (ProgramVariable pv : programVariables) {
    result += pv.getCleanedName();
    if (iterator != pvSize - 1) {
      result += ", ";
    }
    iterator++;
  }
  result += "}";
  return result;
}
std::string PVAliasSet::getVarsStringUsingRawNames() const {
  std::string result = "{";
  int pvSize = programVariables.size();

  int iterator = 0;
  for (ProgramVariable pv : programVariables) {
    result += pv.getRawName();
    if (iterator != pvSize - 1) {
      result += ", ";
    }
    iterator++;
  }
  result += "}";
  return result;
}

std::string PVAliasSet::getMethodsString() const {
  std::set<std::string> methodsSet = methods.getMethods();
  return rlc_util::setToString(methodsSet);
}

int PVAliasSet::getIndex() {
  for (ProgramVariable pv : programVariables) {
    if (pv.hasIndex()) {
      return pv.getIndex();
    }
  }

  return -1;
}

bool PVAliasSet::hasIndex() {
  for (ProgramVariable pv : programVariables) {
    if (pv.hasIndex()) {
      return true;
    }
  }

  return false;
}