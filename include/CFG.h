#ifndef CFG_H
#define CFG_H

#include "Utils.h"

class CFG {
private:
  std::string branchName;
  std::list<CFG *> successors;
  std::list<CFG *> predecessors;
  llvm::SetVector<Instruction *> instructions;

public:
  CFG();
  CFG(std::string branchName, llvm::SetVector<Instruction *> instructions);
  CFG(std::string branchName);

  void addSuccessor(std::string branchName,
                    llvm::SetVector<Instruction *> instructions);
  CFG *addSuccessor(std::string branchName);
  void addSuccessor(CFG *cfg);

  void addPredecessor(std::string branchName,
                      llvm::SetVector<Instruction *> instructions);
  void addPredecessor(std::string branchName);
  void addPredecessor(CFG *cfg);

  void setInstructions(llvm::SetVector<Instruction *> instructions);

  std::string getBranchName();
  std::list<CFG *> getSuccessors();
  std::list<CFG *> getPredecessors();
  llvm::SetVector<Instruction *> getInstructions();
};

#endif
