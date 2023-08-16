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

  // under-approximation of must-aliases
  std::list<ProgramVariable> aliases;

  void fixNameAndIdentifier();

  // bool indicates whether the generated ProgramVariable is actually an
  // identifying variable in the program whose name in the IR begins with a % or
  // @ or not.
  /* this is relevant in the following case:
  in the IR the instruction 'store i32 0, i32* %retval align` the alias
  reasoning previously assumed that whatever we were storing into %retval would
  be a variable/identifier and that we should store the 0 as a ProgramVariable
  that's aliased to %retval, but it not correct to store the number zero as a
  variable. additionally, if there's also the `instruction %0 = load i8**, i8***
  %p, align 8, !dbg !23`, we should be aliasing %0 with %p and their cleaned
  names would be 0 and p, but the program had would think the %0 was referring
  to the ProgramVariable 0, messing up the aliasing reasoning
  */
  bool varIsIdentifier;

public:
  ProgramVariable(Value *value);

  // for struct variables
  ProgramVariable(Value *value, int index);

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

  bool isIdentifier();

  // returns aliases as their string representation only for aliases that are
  // given names in the LLVM IR. intermediate variables like %7, %8 are unnamed;
  // variables present in the C code like %str are named
  std::set<std::string> getNamedAliases(bool cleanNames);

  // returns both named and unnamed aliases
  std::set<std::string> getAllAliases(bool cleanNames);
};

#endif