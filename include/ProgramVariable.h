#ifndef PROGRAM_VARIABLE_H
#define PROGRAM_VARIABLE_H

#include "RunAnalysis.h"
#include "Utils.h"

class ProgramVariable {
private:
  Value *value;

  // name is llvm IR name without % or @
  std::string cleanedName;

  // name is llvm IR name maybe with % or @
  std::string rawName;

  // underapproximation of must-aliases
  std::list<ProgramVariable> aliases;

public:
  ProgramVariable(Value *value);

  std::string getRawName();
  std::string getCleanedName();
  Value *getValue();

  // IR names like %7 are considered unnamed and program names like %str are
  // named
  bool hasProgramName();

  // checks if value pointers are the same
  bool equalsValue(Value *otherValue);

  // checks if value's name (without % or @) equals other name
  bool equalsCleanedName(std::string otherName);

  // checks if value's name (maybe with % or @) equals other name
  bool equalsRawName(std::string otherRawName);

  void addAlias(ProgramVariable pv);

  std::set<std::string> getNamedAliases(bool cleanNames);
  std::set<std::string> getAllAliases(bool cleanNames);
};

#endif