#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "DataflowPass.h"
#include "StructFieldToIndexMap.h"

class TestRunner {
  public:
    // if in the tests the branch is not specified, it will default to
    // lastBranchName
    static bool runTests(const std::string functionName,
                         const std::string lastBranchName,
                         FullFile expectedResult, ProgramFunction receivedResult, StructFieldToIndexMap structFieldToIndexMap);
    static FullFile buildExpectedResults(std::string testName,
                                         std::string passName);
};

#endif