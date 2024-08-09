#ifndef UNWRAP_VALUE_POINTER_TO_STRUCT_TESTER_H
#define UNWRAP_VALUE_POINTER_TO_STRUCT_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests the rlc_dataflow::unwrapValuePointerToStruct function by
// calling the function on each line of an IR program and checking whether
// the result is appropriately NULL or non-NULL
class UnwrapValuePointerToStructTester : public UtilFunctionTester {
  public:
    UnwrapValuePointerToStructTester();
    bool runTest();

};

#endif