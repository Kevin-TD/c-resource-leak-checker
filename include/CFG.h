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
    // any unique functionality. "no instructions stored" would imply that the
    // edge is (and remains) empty but in Analysis.cpp, where we use this overload,
    // the edge will always get filled with instructions as we explicitly look
    // for the instructions in each branch and each branch's successors.
    // so this brings about unnecessary confusion.
    CFG *addSuccessor(std::string branchName);
    void addSuccessor(CFG *cfg);

    void setInstructions(llvm::SetVector<Instruction *> instructions);

    std::string getBranchName();
    std::list<CFG *> getSuccessors();
    llvm::SetVector<Instruction *> getInstructions();
};

#endif
