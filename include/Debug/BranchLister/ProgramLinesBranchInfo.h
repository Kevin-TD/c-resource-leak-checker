#include "Debug/BranchLister/LineBranchInfo.h"

#ifndef PROGRAM_LINES_BRANCH_INFO_H
#define PROGRAM_LINES_BRANCH_INFO_H

// stores branch information about an entire program and
// generates a commented C file listing the branch names
// at certain locations in the code
class ProgramLinesBranchInfo {
  private:
    std::vector<LineBranchInfo> lineBranches;

    // return true iff there exists another LBI stored with the same branch name
    // and function name as otherLBI
    bool exists(const LineBranchInfo otherLBI) const;

  public:
    ProgramLinesBranchInfo();

    // gets a stored LBI based on given line number. if there is no such
    // LBI with line number lineNumber, NULL is returned. otherwise,
    // a reference to the LBI is returned.
    LineBranchInfo* getLBI(unsigned lineNumber);

    // analyzes the branches of a function and stores all line branch information from it.
    // comments are generated only if a function has more than 1 branch. for full program
    // coverage, all functions from a program should be passed into this method
    void add(Function& function);

    // adds comments about branch information to the file being referenced
    // by cFileName (branch information is stored by calls to add).
    // if overwrite is true, the contents of the file will
    // be replaced by the commented version
    void generate(std::string cFileName, bool overwrite);
};

#endif