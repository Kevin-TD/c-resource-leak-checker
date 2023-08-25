#ifndef PROGRAM_FUNCTION_H
#define PROGRAM_FUNCTION_H

#include "ProgramRepresentation/ProgramPoint.h"

// reflects a function from the C code. this class manages a set of points that
// make up a function
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

  // returns a program point based off name and, if addNewIfNotFound is true,
  // creates a new one if it was not found
  ProgramPoint getProgramPoint(std::string name, bool addNewIfNotFound);

  // returns a program point ref based off name and, if addNewIfNotFound is
  // true, creates a new one if it was nout
  ProgramPoint *getProgramPointRef(std::string name, bool addNewIfNotFound);

  std::string getName();
};

#endif