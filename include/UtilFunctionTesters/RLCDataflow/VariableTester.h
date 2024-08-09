#ifndef VARIABLE_TESTER_H
#define VARIABLE_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests the rlc_dataflow::variable function by internally generating an IR file,
// creating a map of the expected variable names for each instruction,
// then comparing the variable function's results with the map
class VariableTester : public UtilFunctionTester {
  public:
    VariableTester();
    bool runTest();
};

#endif