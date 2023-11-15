#include "ProgramRepresentation/ProgramVariable.h"
#include "Debug.h"

ProgramVariable::ProgramVariable() {}

ProgramVariable::ProgramVariable(std::string cleanedName) {
  this->cleanedName = cleanedName;
  this->index = -1;
}

ProgramVariable::ProgramVariable(Value *value) {
  this->value = value;
  this->rawName = Dataflow::variable(value);
  this->cleanedName = this->rawName;
  this->index = -1;
  this->fixNameAndIdentifier();
}

ProgramVariable::ProgramVariable(Value *value, MethodsSet methods) {
  this->value = value;
  this->rawName = Dataflow::variable(value);
  this->cleanedName = this->rawName;
  this->fixNameAndIdentifier();
  this->methods = methods;
  this->index = -1;
}

ProgramVariable::ProgramVariable(Value *value, int index) {
  this->value = value;
  this->rawName = Dataflow::variable(value) + "." + std::to_string(index);
  this->index = index;
  this->cleanedName = this->rawName;
  this->fixNameAndIdentifier();
}

void ProgramVariable::fixNameAndIdentifier() {
  this->varIsIdentifier = true;
  if (this->cleanedName[0] == '%' || this->cleanedName[0] == '@') {
    this->cleanedName.erase(0, 1);
  } else {
    this->varIsIdentifier = false;
  }
}

std::string ProgramVariable::getRawName() { return this->rawName; }

std::string ProgramVariable::getCleanedName() { return this->cleanedName; }

Value *ProgramVariable::getValue() { return this->value; }

bool ProgramVariable::hasProgramName() { return this->value->hasName(); }

bool ProgramVariable::hasIndex() { return this->index != -1; }

int ProgramVariable::getIndex() { return this->index; }

bool ProgramVariable::isIdentifier() { return this->varIsIdentifier; }

bool ProgramVariable::equalsValue(Value *otherValue) {
  return this->value == otherValue;
}

bool ProgramVariable::equalsCleanedName(std::string otherName) {
  return this->cleanedName.compare(otherName) == 0;
}

bool ProgramVariable::equalsRawName(std::string otherRawName) {
  return this->rawName.compare(otherRawName) == 0;
}

bool ProgramVariable::equals(ProgramVariable other) {
  if (this->getCleanedName() != other.getCleanedName()) {
    return false;
  }

  if (!this->getMethodsSet().equals(other.getMethodsSet())) {
    return false;
  }

  return true;
}

void ProgramVariable::addAlias(ProgramVariable pv) {
  this->aliases.push_back(pv);
}

std::set<std::string> ProgramVariable::getNamedAliases(bool cleanNames) {
  std::set<std::string> namedAliases;

  if (this->hasProgramName()) {
    if (cleanNames) {
      namedAliases.insert(this->cleanedName);
    } else {
      namedAliases.insert(this->rawName);
    }
  }

  for (ProgramVariable pv : this->aliases) {
    if (pv.hasProgramName()) {
      if (cleanNames) {
        namedAliases.insert(pv.getCleanedName());
      } else {
        namedAliases.insert(pv.getRawName());
      }
    }
  }
  return namedAliases;
}

std::set<std::string> ProgramVariable::getAllAliases(bool cleanNames) {
  std::set<std::string> allAliases;

  if (cleanNames) {
    allAliases.insert(this->cleanedName);
  } else {
    allAliases.insert(this->rawName);
  }

  for (ProgramVariable pv : this->aliases) {
    if (cleanNames) {
      allAliases.insert(pv.getCleanedName());
    } else {
      allAliases.insert(pv.getRawName());
    }
  }
  return allAliases;
}

MethodsSet ProgramVariable::getMethodsSet() { return this->methods; }

MethodsSet *ProgramVariable::getMethodsSetRef() { return &this->methods; }

std::list<ProgramVariable> ProgramVariable::getPValiases() {
  return this->aliases;
}

std::list<ProgramVariable> *ProgramVariable::getPValiasesRef() {
  return &this->aliases;
}

std::list<ProgramVariable *> ProgramVariable::generatePVptrAliases() {
  std::list<ProgramVariable *> result;
  for (ProgramVariable &pv : this->aliases) {
    result.push_back(&pv);
  }
  return result;
}

void ProgramVariable::setMethodsSet(MethodsSet methods) {
  this->methods.clearMethods();
  this->methods = methods;
}

void ProgramVariable::setAliases(std::list<ProgramVariable> aliases) {
  this->aliases.clear();
  this->aliases = aliases;
}