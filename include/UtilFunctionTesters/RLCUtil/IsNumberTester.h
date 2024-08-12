#ifndef IS_NUMBER_TESTER_H
#define IS_NUMBER_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_util::isNumber
class IsNumberTester : public UtilFunctionTester {
  public:
    IsNumberTester();
    bool runTest();
};

#endif