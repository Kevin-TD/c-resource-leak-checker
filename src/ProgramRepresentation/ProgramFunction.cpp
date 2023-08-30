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

  errs() << "Error at getProgramPointRef: Program point not found and new "
            "program point not added\n";
  std::exit(EXIT_FAILURE);
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

  errs() << "Error at getProgramPoint: Program point not found and new program "
            "point not added\n";
  std::exit(EXIT_FAILURE);
}

std::string ProgramFunction::getFunctionName() { return this->functionName; }

void ProgramFunction::setProgramPoint(std::string name,
                                      ProgramPoint programPoint) {
  ProgramPoint *programPointRef = this->getProgramPointRef(name, true);
  programPointRef->setProgramVariables(programPoint.getProgramVariables());
}