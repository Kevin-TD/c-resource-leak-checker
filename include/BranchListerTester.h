#ifndef BRANCH_LISTER_TESRTER_H
#define BRANCH_LISTER_TESRTER_H

#include "Utils.h"
#include "Debug/BranchLister/ProgramLinesBranchInfo.h"

/* tests `ProgramLineBranchesInfo`. lines of language of verification looks like the following:
- `function=<str> line=<unsigned> branch=<str> preds=<set<str>>`
- e.g., `function=main line=12 branch=main preds={}`
there must not be any spaces anywhere else and each attribute (function, line, branch, or preds)
can be put in any order.
*/
class BranchListerTester {
  public:
    // returns EXIT_SUCCESS if tests passed and EXIT_FAILURE if failed.
    // testName should simply refer to the name of a folder in the
    // test directory. e.g., "if_lub_test".
    static bool runTest(const std::string& testName, const std::string& fnName, ProgramLinesBranchInfo& programLinesBranchInfo);
};

#endif