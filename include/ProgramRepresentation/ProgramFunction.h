#ifndef PROGRAM_FUNCTION_H
#define PROGRAM_FUNCTION_H

#include "ProgramRepresentation/ProgramPoint.h"

class ProgramFunction {
private:
  std::list<ProgramPoint> programPoints;

  // refers to the name of the function
  std::string name;

public:
  ProgramFunction(); 
  ProgramFunction(std::string name); 

  void addProgramPoint(ProgramPoint programPoint);

  void setProgramPoint(std::string name, ProgramPoint programPoint);

  std::list<ProgramPoint> getProgramPoints();

  ProgramPoint getProgramPoint(std::string name); 
  ProgramPoint* getProgramPointRef(std::string name); 

  std::string getName(); 
};

#endif