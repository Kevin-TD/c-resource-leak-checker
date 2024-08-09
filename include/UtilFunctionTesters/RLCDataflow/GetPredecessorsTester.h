#ifndef GET_PREDECESSORS_TESTER_H
#define GET_PREDECESSORS_TESTER_H

#include "UtilFunctionTesters/UtilFunctionTester.h"

// tests rlc_dataflow::getPredecessors by checking the number of
// predecessors for each instruction of an IR file for accuracy
class GetPredecessorsTester : public UtilFunctionTester {
  public:
    GetPredecessorsTester();
    bool runTest();
};

#endif