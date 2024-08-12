#ifndef GET_SUCCESSORS_TESTER_H
#define GET_SUCCESSORS_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_dataflow::getSuccessors by checking the number of
// successors for each instruction of an IR file for accuracy

class GetSuccessorsTester : public UtilFunctionTester {
  public:
    GetSuccessorsTester();
    bool runTest();
};

#endif