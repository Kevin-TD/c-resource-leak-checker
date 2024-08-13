#ifndef STARTS_WITH_TESTER_H
#define STARTS_WITH_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_util::startsWith
class StartsWithTester : public UtilFunctionTester {
  public:
    StartsWithTester();
    bool runTest();
};


#endif