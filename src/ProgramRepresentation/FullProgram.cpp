#include "ProgramRepresentation/FullProgram.h"

FullProgram::FullProgram() {}

FullProgram::FullProgram(std::string name) { this->name = name; }

void FullProgram::addProgramFunction(ProgramFunction programFunction) {
  this->programFunctions.push_back(programFunction);
}

std::list<ProgramFunction> FullProgram::getProgramFunctions() {
  return this->programFunctions;
}

std::string FullProgram::getName() { return this->name; }

ProgramFunction FullProgram::getProgramFunction(std::string name) {
  for (ProgramFunction &programFunction : this->programFunctions) {
    if (programFunction.getName() == name) {
      return programFunction;
    }
  }
  ProgramFunction newProgramFunction = ProgramFunction(name);
  this->addProgramFunction(newProgramFunction);
  return newProgramFunction;
}

ProgramFunction *FullProgram::getProgramFunctionRef(std::string name) {
  for (ProgramFunction &programFunction : this->programFunctions) {
    if (programFunction.getName() == name) {
      return &programFunction;
    }
  }
  ProgramFunction newProgramFunction = ProgramFunction(name);
  this->addProgramFunction(newProgramFunction);
  return getProgramFunctionRef(name);
}