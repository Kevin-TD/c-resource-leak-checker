#include "CFG.h"

CFG::CFG() {
    this->branchName = "";
}

CFG::CFG(std::string branchName) {
    this->branchName = branchName;
}

CFG::CFG(std::string branchName, llvm::SetVector<Instruction *> instructions) {
    this->branchName = branchName;
    this->instructions = instructions;
}

void CFG::addSuccessor(std::string branchName,
                       llvm::SetVector<Instruction *> instructions) {
    this->successors.push_back(new CFG(branchName, instructions));
}

CFG *CFG::addSuccessor(std::string branchName) {
    CFG *cfg = new CFG(branchName);
    this->successors.push_back(cfg);
    return cfg;
}

void CFG::addSuccessor(CFG *tree) {
    this->successors.push_back(tree);
}

std::string CFG::getBranchName() {
    return this->branchName;
}

std::list<CFG *> CFG::getSuccessors() {
    return this->successors;
}

void CFG::setInstructions(llvm::SetVector<Instruction *> instructions) {
    this->instructions = instructions;
}

llvm::SetVector<Instruction *> CFG::getInstructions() {
    return this->instructions;
}
