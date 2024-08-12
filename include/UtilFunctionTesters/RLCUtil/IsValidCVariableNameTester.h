#ifndef IS_VALID_C_VARIABLE_NAME_TESTER_H
#define IS_VALID_C_VARIABLE_NAME_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_util::isValidCVariableName
class IsValidCVariableNameTester : public UtilFunctionTester {
  public:
    IsValidCVariableNameTester();
    bool runTest();
};

#endif