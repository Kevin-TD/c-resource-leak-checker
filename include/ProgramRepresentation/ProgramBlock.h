#ifndef PROGRAM_BLOCK_H
#define PROGRAM_BLOCK_H

#include "ProgramRepresentation/ProgramPoint.h"

// reflects a branch that holds a continuous set of instructions in the IR
// that will always be run consecutively. this class manages
// a block's program variables using a set of alias sets. it is effectively a
// wrapper for a continuous list of points

class ProgramPoint;

class ProgramBlock {
  private:
    std::list<ProgramBlock *> successors;
    std::list<ProgramPoint *> points;
    Value *returnValue;

    // the name is same as the branch name that shows up in the IR
    std::string blockName;

    // Helps assign parent function to its points
    ProgramFunction *parent;

  public:
    friend class DataflowPass;
    friend class ProgramFunction;
    // debugging function that lists variables and number of points
    static void logoutProgramBlock(const ProgramBlock &block);

    // debugging function that lists variables and number of points
    static void logoutProgramBlock(const ProgramBlock *block);

    ProgramBlock();
    //This tracks whether the block has reached fixed point
    bool fixed;
    ProgramBlock(std::string blockName);

    // Returns the return value associated with this program block for the Scope Analyzer
    // to ignore out of scope for returned variables
    // Likely will need to be refactored later so all resources that are owned by
    // other parts in the program are returned because their ownership status imply
    // they will be handled elsewhere
    //
    // This is done by block rather than by function because a function may return different aliases
    // while a block is guaranteed to have one or zero return values. In the case of no return values
    // a NULL ptr is returned.
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

    PVAliasSet *getPVASRefFromValue(Value* value);

    ProgramPoint *getPoint(unsigned int line, bool addNew);
    ProgramPoint *getLast();

    // returns a list of successors
    std::list<ProgramBlock *> getSuccessors();

    std::string getBlockName() const;

    void update(int point);

    // Returns a list of contained program points
    std::list<ProgramPoint *> getPoints();
};

#endif
