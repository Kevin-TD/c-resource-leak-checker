#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "DataflowPass.h"

class TestRunner {
public:
  static bool runTests(MappedMethods expectedResult,
                       MappedMethods receivedResult);
  static MappedMethods buildExpectedResults(std::string testName,
                                            std::string passName);
};

#endif