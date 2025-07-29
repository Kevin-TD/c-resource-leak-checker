#ifndef CFG_H
#define CFG_H

#include "Utils.h"

// implementation of a control flow graph (cfg)
// used to represent the control flow of an entire file (`FullFile`)
// see: https://en.wikipedia.org/wiki/Control-flow_graph
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
    // TODO: remove this as it is more confusing than useful and does not contain
    // any unique functionality
    CFG *addSuccessor(std::string branchName);
    void addSuccessor(CFG *cfg);

    void setInstructions(llvm::SetVector<Instruction *> instructions);

    std::string getBranchName();
    std::list<CFG *> getSuccessors();
    llvm::SetVector<Instruction *> getInstructions();
};

#endif
