#ifndef REMOVE_WHITESPACE_TESTER_H
#define REMOVE_WHITESPACE_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_util::removeWhitespace
class RemoveWhitespaceTester : public UtilFunctionTester {
  public:
    RemoveWhitespaceTester();
    bool runTest();
};

#endif