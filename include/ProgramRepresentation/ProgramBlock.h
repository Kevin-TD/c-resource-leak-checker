#ifndef PROGRAM_BLOCK_H
#define PROGRAM_BLOCK_H

#include "ProgramRepresentation/ProgramPoint.h"

// reflects a branch that holds some instructions in the IR. this class manages
// a block's program variables using a set of alias sets. it is effectively a
// wrapper for a continuous list of points
class ProgramBlock {
  private:
    std::list<ProgramBlock *> successors;
    std::list<ProgramPoint *> points;
    Value *returnValue;

    // the name is same as the branch name that shows up in the IR
    std::string blockName;

  public:
    friend class DataflowPass;
    friend class ProgramFunction;
    // debugging function that lists variables and number of points
    static void logoutProgramBlock(const ProgramBlock &block);

    // debugging function that lists variables and number of points
    static void logoutProgramBlock(const ProgramBlock *block);

    ProgramBlock();
    ProgramBlock(std::string blockName);

    // Returns the return value associated with this program point
    // Likely will need to be remodeled later so all resources that are owned by
    // other parts in the program are returned
    Value *getReturnValue();

    // copies the alias from the last programPoint of its predecessor into a new instance
    // Performs LUB if there are multiple predecessors
    ProgramBlock(std::string pointName, ProgramBlock *programPoint);

    // Checks if there is an alias set that contains this var
    bool varExists(ProgramVariable programVar);

    // adds a new successor program block
    void addSuccessor(ProgramBlock *successor);

    // adds a new program point
    void add(ProgramPoint *point);

    ProgramPoint *getPoint(unsigned int line);

    // returns a list of successors
    std::list<ProgramBlock *> getSuccessors();

    std::string getBlockName() const;

    // Returns a list of contained program points
    std::list<ProgramPoint> getPoints();
};

#endif
