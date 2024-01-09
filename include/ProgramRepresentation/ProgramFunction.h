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

    // TODO: push this to separate pr
    // searches every program point to find the alias set with that value pointer. this
    // method should only be used if you need access to an alias set that does not
    // exist in your current branch, but in a preceding one.
    /*
    Example situation:

    struct M {
      char* x;
      char* y;
    };

    int main() {
      struct M m;
      m.x = (char*)malloc(15);
      m.y = m.x;
      [[ m.x and m.y are aliased ]]

      if (getchar()) {
        free(m.y);
        [[ in the IR, we detect that we are
        referencing field y of struct M via a gepInst (effectively breaks down
        a struct into its fields, see docs: https://llvm.org/docs/GetElementPtr.html)

        we need a ref (Value*) to that struct to build this new program variable m.y and add
        necessary aliases. that ref is in the entry/previous program point, however. thus,
        to access it, we call this method. ]]
      }
    }
    */
    PVAliasSet *getPVASRefFromValue(Value* value);

    std::string getFunctionName() const;
};

#endif