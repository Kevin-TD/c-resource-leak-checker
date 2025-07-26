#ifndef IR_STRUCT_NAME_EQUALS_C_STRUCT_NAME_TESTER_H
#define IR_STRUCT_NAME_EQUALS_C_STRUCT_NAME_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_dataflow::IRstructNameEqualsCstructName by checking if
// certain structs exist in a specific ir file in the test directory
class IRstructNameEqualsCstructNameTester : public UtilFunctionTester {
  public:
    IRstructNameEqualsCstructNameTester();
    bool runTest();
};

#endif