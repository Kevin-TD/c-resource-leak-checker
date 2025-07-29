#ifndef FULL_FILE_H
#define FULL_FILE_H

#include "ProgramRepresentation/ProgramFunction.h"

/* reflects a file. this class manages all the functions that make up a single
file. how everything in `ProgramRepresentation` is structured is as follows:

- a `FullFile` is composed of functions (`ProgramFunction` s) that the user declares.
- `ProgramFunction` s are composed of LLVM IR branches (`ProgramPoints` s)
- and these branches contain information mapping each variable (and any of its aliases)
to a set of methods called on them, which are the `DisjointPVAliasSets`.
- these disjoint sets are composed of `PVAliasSet` s, which holds a set of program variables
(typed `ProgramVariable`) which are must-aliases of each other, and the set of methods
called on them (typed `MethodSet`)
- `ProgramVariable` s are isomorphic to the
set of variables seen in the user's C code or is a variable
defined only in the LLVM IR code – usually an intermediate variable.
variables)
- finally, `MethodsSet` is a container for a set of methods.
- the hierarchy may be "ordered" as follows:
`FullFile` > `ProgramFunction` > `ProgramPoint` > `PVAliasSet` > (`ProgramVariable, MethodsSet`)

(tasteless pedantry: here ">" is really just a left-associative binary relation saying that
"a > b" means a is composed of a set of b's.)
*/
class FullFile {
  private:
    std::list<ProgramFunction> programFunctions;

    std::string fileName;

  public:
    FullFile();
    FullFile(std::string fileName);

    void addProgramFunction(ProgramFunction programFunction);

    std::list<ProgramFunction> getProgramFunctions() const;

    // returns the program function with name `functionName` and, if `addNewIfNotFound` is
    // true, creates a new one if it was not. if `addNewIfNotFound` is false and the
    // function was not found, the program fails & exits
    ProgramFunction getProgramFunction(const std::string &functionName,
                                       bool addNewIfNotFound);

    // returns the program function ref with name `functionName` and, if `addNewIfNotFound`
    // is true, creates a new one if it was not. if `addNewIfNotFound` is false and
    // the function was not found, the program fails & exits
    ProgramFunction *getProgramFunctionRef(const std::string &functionName,
                                           bool addNewIfNotFound);

    std::string getFileName() const;
};

#endif