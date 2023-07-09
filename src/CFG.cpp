#include "CFG.h"

CFG::CFG() { this->branchName = ""; }

CFG::CFG(std::string branchName) { this->branchName = branchName; }

CFG::CFG(std::string branchName, llvm::SetVector<Instruction *> instructions) {
  this->branchName = branchName;
  this->instructions = instructions;
}

void CFG::addSuccessor(std::string branchName,
                       llvm::SetVector<Instruction *> instructions) {
  this->successors.insert(new CFG(branchName, instructions));
}

CFG *CFG::addSuccessor(std::string branchName) {
  CFG *cfg = new CFG(branchName);
  this->successors.insert(cfg);
  return cfg;
}

void CFG::addSuccessor(CFG *tree) { this->successors.insert(tree); }

void CFG::addPredecessor(std::string branchName,
                         llvm::SetVector<Instruction *> instructions) {
  this->predecessors.insert(new CFG(branchName, instructions));
}

void CFG::addPredecessor(std::string branchName) {
  this->predecessors.insert(new CFG(branchName));
}

void CFG::addPredecessor(CFG *cfg) { this->predecessors.insert(cfg); }

std::string CFG::getBranchName() { return this->branchName; }

std::set<CFG *> CFG::getSuccessors() { return this->successors; }

std::set<CFG *> CFG::getPredecessors() { return this->predecessors; }

void CFG::setInstructions(llvm::SetVector<Instruction *> instructions) {
  this->instructions = instructions;
}

llvm::SetVector<Instruction *> CFG::getInstructions() {
  return this->instructions;
}