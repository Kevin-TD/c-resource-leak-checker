#ifndef SLICE_STRING_TESTER_H
#define SLICE_STRING_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_util::sliceString
class SliceStringTester : public UtilFunctionTester {
  public:
    SliceStringTester();
    bool runTest();
};


#endif