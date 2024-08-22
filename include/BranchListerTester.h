#ifndef BRANCH_LISTER_TESRTER_H
#define BRANCH_LISTER_TESRTER_H

#include "Utils.h"
#include "Debug/BranchLister/ProgramLinesBranchInfo.h"

class BranchListerTester {
  public:
    // returns EXIT_SUCCESS if tests passed and EXIT_FAILURE if failed.
    // testName should simply refer to the name of a folder in the
    // test directory. e.g., "if_lub_test".
    static bool runTest(const std::string& testName, ProgramLinesBranchInfo& programLinesBranchInfo);
};

#endif