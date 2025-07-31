#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "DataflowPass.h"
#include "StructFieldToIndexMap.h"

/* runs test as expected to be used for the must call and called method
pass results. the language of specification looks like:

- `pass=<str> function=<str> branch=<str> var=<str> methods=<set<str>>`

test arguments are written in one line and can be put in any order.
e.g., `pass=MustCall function=main branch=entry var=s methods={free}`.
there must not be any spaces anywhere else, including in methods
argument. to add more methods to method arg, it would look like `methods={x,y}`.
TODO: rename to DataflowPassTester?
*/
class TestRunner {
  public:
    // if in the tests the branch is not specified, it will default to
    // `lastBranchName`
    static bool runTests(const std::string functionName,
                         const std::string lastBranchName,
                         FullFile expectedResult, ProgramFunction receivedResult, StructFieldToIndexMap structFieldToIndexMap);
    static FullFile buildExpectedResults(std::string testName,
                                         std::string passName);
};

#endif