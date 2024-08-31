#ifndef LINE_NUMBER_TO_L_VALUE_TESTER_H
#define LINE_NUMBER_TO_L_VALUE_TESTER_H

#include "Utils.h"
#include "LineNumberToLValueMap.h"

class LineNumberToLValueTester {
  public:
    static bool runTest(const std::string& testName, LineNumberToLValueMap& lineNumberToLValueMap);
};

#endif