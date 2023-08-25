#include "ProgramRepresentation/ProgramFunction.h"
#include "ProgramRepresentation/ProgramVariable.h"
#include "Debug.h"


ProgramFunction::ProgramFunction(std::string name) {
    this->name = name; 
}

ProgramFunction::ProgramFunction() {
    
}

void ProgramFunction::addProgramPoint(ProgramPoint programPoint) {
    this->programPoints.push_back(programPoint); 
}

std::list<ProgramPoint> ProgramFunction::getProgramPoints() {
    return this->programPoints; 
}

ProgramPoint* ProgramFunction::getProgramPointRef(std::string name) {
    for (ProgramPoint& programPoint : this->programPoints) {
        if (programPoint.getName() == name) {
            return &programPoint; 
        }
    }
    ProgramPoint newProgramPoint = ProgramPoint(name);
    this->addProgramPoint(newProgramPoint);
    return this->getProgramPointRef(name);
}

ProgramPoint ProgramFunction::getProgramPoint(std::string name) {
    for (ProgramPoint& programPoint : this->programPoints) {
        if (programPoint.getName() == name) {
            return programPoint; 
        }
    }
    ProgramPoint newProgramPoint = ProgramPoint(name);
    this->addProgramPoint(newProgramPoint);
    return newProgramPoint;
}

std::string ProgramFunction::getName() {
    return this->name; 
}

void ProgramFunction::setProgramPoint(std::string name, ProgramPoint programPoint) {
    ProgramPoint* programPointRef = this->getProgramPointRef(name); 
    programPointRef->setProgramVariables(programPoint.getProgramVariables());
}