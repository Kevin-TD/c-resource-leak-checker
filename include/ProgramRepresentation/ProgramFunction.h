#ifndef PROGRAM_FUNCTION_H
#define PROGRAM_FUNCTION_H

#include "ProgramRepresentation/ProgramPoint.h"

// reflects a function from the C code. this class manages a set of points that
// make up a function
class ProgramFunction {
private:
  std::list<ProgramPoint> programPoints;

  std::string functionName;

public:
  static void logoutPF(ProgramFunction &pf);
  ProgramFunction();
  ProgramFunction(std::string functionName);

  void addProgramPoint(ProgramPoint programPoint);

  void setProgramPoint(std::string pointName, ProgramPoint programPoint);

  std::list<ProgramPoint> getProgramPoints();

  // returns a program point based off pointName and, if addNewIfNotFound is
  // true, creates a new one if it was not found. if addNewIfNotFound is false
  // and the point was not found, the program fails & exits
  ProgramPoint getProgramPoint(std::string pointName, bool addNewIfNotFound);

  // returns a program point ref based off name and, if addNewIfNotFound is
  // true, creates a new one if it was not. if addNewIfNotFound is false and the
  // point was not found, the program fails & exits
  ProgramPoint *getProgramPointRef(std::string pointName,
                                   bool addNewIfNotFound);

  std::string getFunctionName();
};

#endif