#ifndef PROGRAM_FUNCTION_H
#define PROGRAM_FUNCTION_H

#include "ProgramRepresentation/ProgramPoint.h"
#include "ProgramRepresentation/ProgramBlock.h"
#include "Annotations/AnnotationHandler.h"

// reflects a function from the C code. this class manages a set of blocks that
// make up a function and has a counter taht is used to assign IDs to alias sets within it

class ProgramFunction {
    friend class DataflowPass;
  private:
    std::list<ProgramBlock> programBlocks;

    // When the must call and called methods passes run, there are two separate functions, these need to have their
    // currentAliasNums sync'd, so a pointer to a paired function is used to update the other whenever one
    // generates a new ID
    ProgramFunction *twin;

    AnnotationHandler a;
    // Tracks the next unique id for aliases in the function, updated whenever a new alias is created
    int currentAliasNum = 0;

    std::string functionName;

  public:
    // debugging function that lists program blocks which are a set of continuous program points
    static void logoutProgramFunction(ProgramFunction &programFunction,
                                      bool logMethods);

    ProgramFunction();
    ProgramFunction(std::string functionName);

    bool checkFixed();

    void pair(ProgramFunction *p);

    AnnotationHandler *getAnnotationHandler();
    void setAnnotationHandler(AnnotationHandler a);
    int currID();

    void addProgramBlock(ProgramBlock programBlock);

    void setProgramBlock(std::string blockName, ProgramBlock programBlock);

    std::list<ProgramBlock> getProgramBlocks() const;

    ProgramFunction *deepCopy();

    int getNewID();

    void resetID();

    // returns a program block based off pointName and, if addNewIfNotFound is
    // true, creates a new one if it was not found. if addNewIfNotFound is false
    // and the point was not found, the program fails & exits
    ProgramBlock getProgramBlock(const std::string &blockName,
                                 bool addNewIfNotFound);

    // returns a program point ref based off name and, if addNewIfNotFound is
    // true, creates a new one if it was not. if addNewIfNotFound is false and the
    // point was not found, the program fails & exits
    ProgramBlock *getProgramBlockRef(const std::string &blockName,
                                     bool addNewIfNotFound);

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

    std::string getFunctionName() const;
};

#endif
