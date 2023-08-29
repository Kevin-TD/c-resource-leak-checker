#include "ProgramRepresentation/ProgramFunction.h"
#include "Debug.h"
#include "ProgramRepresentation/ProgramVariable.h"

ProgramFunction::ProgramFunction(std::string functionName) {
  this->functionName = functionName;
}

ProgramFunction::ProgramFunction() {}

void ProgramFunction::addProgramPoint(ProgramPoint programPoint) {
  this->programPoints.push_back(programPoint);
}

std::list<ProgramPoint> ProgramFunction::getProgramPoints() {
  return this->programPoints;
}

ProgramPoint *ProgramFunction::getProgramPointRef(std::string pointName,
                                                  bool addNewIfNotFound) {
  for (ProgramPoint &programPoint : this->programPoints) {
    if (programPoint.getPointName() == pointName) {
      return &programPoint;
    }
  }

  if (addNewIfNotFound) {
    ProgramPoint newProgramPoint = ProgramPoint(pointName);
    this->addProgramPoint(newProgramPoint);
    return &this->programPoints.back();
  }

  // TODO: program should error and halt if this case happens
  return NULL;
}

ProgramPoint ProgramFunction::getProgramPoint(std::string pointName,
                                              bool addNewIfNotFound) {
  for (ProgramPoint &programPoint : this->programPoints) {
    if (programPoint.getPointName() == pointName) {
      return programPoint;
    }
  }

  if (addNewIfNotFound) {
    ProgramPoint newProgramPoint = ProgramPoint(pointName);
    this->addProgramPoint(newProgramPoint);
    return this->programPoints.back();
  }

  // TODO: program should error and halt if this case happens
  return ProgramPoint();
}

std::string ProgramFunction::getFunctionName() { return this->functionName; }

void ProgramFunction::setProgramPoint(std::string name,
                                      ProgramPoint programPoint) {
  ProgramPoint *programPointRef = this->getProgramPointRef(name, true);
  programPointRef->setProgramVariables(programPoint.getProgramVariables());
}