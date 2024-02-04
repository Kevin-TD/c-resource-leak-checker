#include "Debug/BranchLister/LineBranchInfo.h"
#include "Debug.h"

LineBranchInfo::LineBranchInfo(unsigned lineNumber, std::string branchName, std::vector<llvm::Instruction *> branchPredecessors, std::string functionName) {
    this->lineNumber = lineNumber;
    this->branchName = branchName;
    this->branchPredecessors = branchPredecessors;
    this->functionName = functionName;
}

std::string LineBranchInfo::getBranchName() const {
    return this->branchName;
}

std::string LineBranchInfo::getFunctionName() const {
    return this->functionName;
}

unsigned LineBranchInfo::getLineNumber() const {
    return this->lineNumber;
}

std::string LineBranchInfo::toString() const {
    std::string branchPredsString;

    int numberOfBranchPreds = this->branchPredecessors.size();
    int iterator = 0;
    for (auto pred : this->branchPredecessors) {
        std::string predName = pred->getParent()->getName().str();
        if (predName != this->branchName) {
            branchPredsString += predName;
        }
        if (iterator != numberOfBranchPreds - 1) {
            branchPredsString += ", ";
        }
        iterator++;
    }

    if (branchPredsString != "") {
        branchPredsString = ", preds = {" + branchPredsString + "}";
    }

    return branchName + branchPredsString;
}