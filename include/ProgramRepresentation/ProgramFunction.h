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
  // debugging function that lists program points and methods called (methods
  // logged if logMethods is true) of a program function
  static void logoutProgramFunction(ProgramFunction &programFunction,
                                    bool logMethods);

  ProgramFunction();
  ProgramFunction(std::string functionName);

  void addProgramPoint(ProgramPoint programPoint);

  void setProgramPoint(std::string pointName, ProgramPoint programPoint);

  std::list<ProgramPoint> getProgramPoints() const;

  // returns a program point based off pointName and, if addNewIfNotFound is
  // true, creates a new one if it was not found. if addNewIfNotFound is false
  // and the point was not found, the program fails & exits
  ProgramPoint getProgramPoint(const std::string &pointName,
                               bool addNewIfNotFound);

  // returns a program point ref based off name and, if addNewIfNotFound is
  // true, creates a new one if it was not. if addNewIfNotFound is false and the
  // point was not found, the program fails & exits
  ProgramPoint *getProgramPointRef(const std::string &pointName,
                                   bool addNewIfNotFound);

  std::string getFunctionName() const;
};

#endif