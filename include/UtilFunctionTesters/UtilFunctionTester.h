#ifndef UTIL_FUNCTION_TESTER_H
#define UTIL_FUNCTION_TESTER_H

#include "RunAnalysis.h"
#include "Debug.h"
#include "Utils.h"

// a purely abstract class to be extended in order to represent
// the fact that it is a util function that needs to have a test
// ran
class UtilFunctionTester {
  public:
    // should return true iff test passed and false iff otherwise
    virtual bool runTest() = 0;
};

#endif