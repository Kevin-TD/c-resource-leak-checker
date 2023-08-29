#include "ProgramRepresentation/FullFile.h"

FullFile::FullFile() {}

FullFile::FullFile(std::string fileName) { this->fileName = fileName; }

void FullFile::addProgramFunction(ProgramFunction programFunction) {
  this->programFunctions.push_back(programFunction);
}

std::list<ProgramFunction> FullFile::getProgramFunctions() {
  return this->programFunctions;
}

std::string FullFile::getFileName() { return this->fileName; }

ProgramFunction FullFile::getProgramFunction(std::string functionName,
                                             bool addNewIfNotFound) {
  for (ProgramFunction &programFunction : this->programFunctions) {
    if (programFunction.getFunctionName() == functionName) {
      return programFunction;
    }
  }

  if (addNewIfNotFound) {
    ProgramFunction newProgramFunction = ProgramFunction(functionName);
    this->addProgramFunction(newProgramFunction);
    return newProgramFunction;
  }

  // TODO: should error here instead
  return ProgramFunction();
}

ProgramFunction *FullFile::getProgramFunctionRef(std::string functionName,
                                                 bool addNewIfNotFound) {
  for (ProgramFunction &programFunction : this->programFunctions) {
    if (programFunction.getFunctionName() == functionName) {
      return &programFunction;
    }
  }

  if (addNewIfNotFound) {
    ProgramFunction newProgramFunction = ProgramFunction(functionName);
    this->addProgramFunction(newProgramFunction);
    return &this->programFunctions.back();
  }

  // TODO: should error here instead
  return NULL;
}