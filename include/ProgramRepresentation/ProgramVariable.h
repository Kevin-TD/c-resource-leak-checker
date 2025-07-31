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

    // if program var is referring to a struct's field, it has an index.
    // otherwise, it is equal to -1
    int fieldIndex;

    // represents an alias set the program variable is apart of. e.g.,
    // if we have two sets {a, b, c} and {x, y, z}, all elements have
    // set number zero. but if we join them together,
    // a, b, c will have set number 0, but x, y, z will have set number 1.
    // all elements are still aliased. this is useful for if we need to
    // perform unaliasing, turning {a, b, c, x, y, z} back into
    // {a, b, c} and {x, y, z}
    unsigned setNumber;

  public:
    ProgramVariable();
    ProgramVariable(Value *value);
    ProgramVariable(std::string cleanedName);

    // for struct variables
    ProgramVariable(Value *value, unsigned fieldIndex);

    std::string getRawName() const;
    std::string getCleanedName();
    Value *getValue();
    int getFieldIndex();

    // returns true iff index does not equal -1, meaning the program var
    // refers to a struct's field
    bool containsStructFieldVar() const;

    // IR names like %7 are considered unnamed and program names like %str are
    // named
    bool hasProgramName() const;

    // checks if value pointers are the same
    bool equalsValue(Value *otherValue) const;

    // checks if value's name (without % or @) equals other name
    bool equalsCleanedName(const std::string &otherName) const;

    // checks if value's name (maybe with % or @) equals other name
    bool equalsRawName(const std::string &otherRawName) const;

    unsigned getSetNumber() const;
    void setSetNumber(unsigned setNumber);

    bool isIdentifier() const;
};

#endif