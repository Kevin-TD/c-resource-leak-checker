#ifndef UTIL_FUNCTION_TESTER_H
#define UTIL_FUNCTION_TESTER_H

#include "RunAnalysis.h"
#include "Debug.h"
#include "Utils.h"

class UtilFunctionTester {
  public:
    // should return true iff test passed and false iff otherwise
    virtual bool runTest() = 0;
};

#endif