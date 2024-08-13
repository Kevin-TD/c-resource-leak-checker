#ifndef BRANCH_LISTER_TESRTER_H
#define BRANCH_LISTER_TESRTER_H

#include "Utils.h"
#include "Debug/BranchLister/ProgramLinesBranchInfo.h"

class BranchListerTester {
  public:
    static bool runTest(const std::string& testName, ProgramLinesBranchInfo& programLinesBranchInfo);
};

#endif