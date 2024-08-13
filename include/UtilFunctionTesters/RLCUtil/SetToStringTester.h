#ifndef SET_TO_STRING_TESTER_H
#define SET_TO_STRING_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// test rlc_util::setToString
class SetToStringTester : public UtilFunctionTester {
  public:
    SetToStringTester();
    bool runTest();
};

#endif