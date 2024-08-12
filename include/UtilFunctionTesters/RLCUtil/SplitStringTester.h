#ifndef SPLIT_STRING_TESTER_H
#define SPLIT_STRING_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"


// tests rlc_util::splitString
class SplitStringTester : public UtilFunctionTester {
  public:
    SplitStringTester();
    bool runTest();
};

#endif