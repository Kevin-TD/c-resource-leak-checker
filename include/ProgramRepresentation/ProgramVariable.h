#ifndef PROGRAM_VARIABLE_H
#define PROGRAM_VARIABLE_H

#include "ProgramRepresentation/MethodsSet.h"
#include "RunAnalysis.h"
#include "Utils.h"

// reflects a variable visible in the C program and/or IR
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
  /* without this, for instructions like `store i32 0, i32* %retval align`, the
  program would think '0' refers to some variable in the program. This can be
  problematic if there's an instruction like `instruction %0 = load i8**, i8***
  %p, align 8, !dbg !23`, which could cause the alias reasoning to mess up and
  mistake the variable %0 with some reference to the '0' program variable
  */
  bool varIsIdentifier;

  MethodsSet methods;

public:
  ProgramVariable();
  ProgramVariable(Value *value);
  ProgramVariable(Value *value, MethodsSet methods);
  ProgramVariable(std::string cleanedName);

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

  bool equals(ProgramVariable other);

  void addAlias(ProgramVariable pv);

  bool isIdentifier();

  // returns aliases as their string representation only for aliases that are
  // given names in the LLVM IR. intermediate variables like %7, %8 are unnamed;
  // variables present in the C code like %str are named
  std::set<std::string> getNamedAliases(bool cleanNames);

  // returns both named and unnamed aliases
  std::set<std::string> getAllAliases(bool cleanNames);

  std::list<ProgramVariable> getPValiases();
  std::list<ProgramVariable> *getPValiasesRef();

  std::list<ProgramVariable *> generatePVptrAliases();

  MethodsSet getMethodsSet();
  MethodsSet *getMethodsSetRef();

  void setMethodsSet(MethodsSet methods);

  void setAliases(std::list<ProgramVariable> aliases);
};

#endif