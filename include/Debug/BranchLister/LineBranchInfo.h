#include "Utils.h"

#ifndef LINE_BRANCH_INFO
#define LINE_BRANCH_INFO

// correlates an instruction into a string that lists
// the branch name and its predecessors
class LineBranchInfo {
  private:
    // represents the line number of the corresponding line in C code for the instruction
    unsigned lineNumber;

    std::string branchName;

    std::vector<llvm::Instruction *> branchPredecessors;

    // refers to the function name the instruction was called in
    std::string functionName;

  public:
    LineBranchInfo(unsigned line, std::string branchName, std::vector<llvm::Instruction *> branchPredecessors, std::string functionName);

    std::string getBranchName() const;
    std::string getFunctionName() const;
    unsigned getLineNumber() const;
    std::set<std::string> getPreds() const;

    std::string toString() const;
};

#endif LINE_BRANCH_INFO