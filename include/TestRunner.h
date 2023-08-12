#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "DataflowPass.h"

class TestRunner {
public:
  static bool runTests(const std::string functionName,
                       FunctionMappedMethods expectedResult,
                       MappedMethods receivedResult);
  static FunctionMappedMethods buildExpectedResults(std::string testName,
                                                    std::string passName);
};

#endif