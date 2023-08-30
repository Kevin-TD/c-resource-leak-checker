#ifndef FULL_FILE_H
#define FULL_FILE_H

#include "ProgramRepresentation/ProgramFunction.h"

// reflects a file. this class manages all the functions that make up a single
// file.
class FullFile {
private:
  std::list<ProgramFunction> programFunctions;

  std::string fileName;

public:
  FullFile();
  FullFile(std::string fileName);

  void addProgramFunction(ProgramFunction programFunction);

  std::list<ProgramFunction> getProgramFunctions();

  // returns a program function based functionName and, if addNewIfNotFound is
  // true, creates a new one if it was not. if addNewIfNotFound is false and the
  // function was not found, the program fails & exits
  ProgramFunction getProgramFunction(std::string functionName,
                                     bool addNewIfNotFound);

  // returns a program function ref based functionName and, if addNewIfNotFound
  // is true, creates a new one if it was not. if addNewIfNotFound is false and
  // the function was not found, the program fails & exits
  ProgramFunction *getProgramFunctionRef(std::string functionName,
                                         bool addNewIfNotFound);

  std::string getFileName();
};

#endif