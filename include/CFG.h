#ifndef CFG_H
#define CFG_H

#include "Utils.h"

// the control flow graph (cfg) represents the connection branches have among themselves
// in a program block, relating branches with their successors. each branch
// has its name, instructions, and successors stored.
class CFG {
  private:
    std::string branchName;
    std::list<CFG *> successors;
    llvm::SetVector<Instruction *> instructions;

  public:
    CFG();
    CFG(std::string branchName, llvm::SetVector<Instruction *> instructions);
    CFG(std::string branchName);

    void addSuccessor(std::string branchName,
                      llvm::SetVector<Instruction *> instructions);
    // returns the added successor, which just a cfg with only a branch name and
    // no instructions stored
    CFG *addSuccessor(std::string branchName);
    void addSuccessor(CFG *cfg);

    void setInstructions(llvm::SetVector<Instruction *> instructions);

    std::string getBranchName();
    std::list<CFG *> getSuccessors();
    llvm::SetVector<Instruction *> getInstructions();
};

#endif
