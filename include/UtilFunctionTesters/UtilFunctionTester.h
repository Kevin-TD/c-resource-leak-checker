#ifndef UTIL_FUNCTION_TESTER_H
#define UTIL_FUNCTION_TESTER_H

#include "RunAnalysis.h"
#include "Utils.h"

class UtilFunctionTester {
    // returns true iff test passed and false iff otherwise
  public:
    virtual bool runTest() = 0;
};

#endif