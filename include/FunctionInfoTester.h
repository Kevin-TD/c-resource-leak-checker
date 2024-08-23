#ifndef FUNCTION_INFO_TESTER_H
#define FUNCTION_INFO_TESTER_H

#include "FunctionInfosManager.h"

class FunctionInfoTester {
  public:
    // returns EXIT_SUCCESS if tests passed and EXIT_FAILURE if failed
    static bool runTest(const std::string& testName, FunctionInfosManager& functionInfosManager);
};

#endif