#include "ProgramRepresentation/ProgramFunction.h"
#include "Debug.h"
#include "ProgramRepresentation/ProgramVariable.h"

ProgramFunction::ProgramFunction(std::string name) { this->name = name; }

ProgramFunction::ProgramFunction() {}

void ProgramFunction::addProgramPoint(ProgramPoint programPoint) {
  this->programPoints.push_back(programPoint);
}

std::list<ProgramPoint> ProgramFunction::getProgramPoints() {
  return this->programPoints;
}

ProgramPoint *ProgramFunction::getProgramPointRef(std::string name,
                                                  bool addNewIfNotFound) {
  for (ProgramPoint &programPoint : this->programPoints) {
    if (programPoint.getName() == name) {
      return &programPoint;
    }
  }

  if (addNewIfNotFound) {
    ProgramPoint newProgramPoint = ProgramPoint(name);
    this->addProgramPoint(newProgramPoint);
    return &this->programPoints.back();
  }

  return new ProgramPoint();
}

ProgramPoint ProgramFunction::getProgramPoint(std::string name,
                                              bool addNewIfNotFound) {
  for (ProgramPoint &programPoint : this->programPoints) {
    if (programPoint.getName() == name) {
      return programPoint;
    }
  }

  if (addNewIfNotFound) {
    ProgramPoint newProgramPoint = ProgramPoint(name);
    this->addProgramPoint(newProgramPoint);
    return this->programPoints.back();
  }

  return ProgramPoint();
}

std::string ProgramFunction::getName() { return this->name; }

void ProgramFunction::setProgramPoint(std::string name,
                                      ProgramPoint programPoint) {
  ProgramPoint *programPointRef = this->getProgramPointRef(name, true);
  programPointRef->setProgramVariables(programPoint.getProgramVariables());
}