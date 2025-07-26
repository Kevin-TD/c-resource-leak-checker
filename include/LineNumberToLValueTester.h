#ifndef LINE_NUMBER_TO_L_VALUE_TESTER_H
#define LINE_NUMBER_TO_L_VALUE_TESTER_H

#include "Utils.h"
#include "LineNumberToLValueMap.h"

/* tests `LineNumberToLValueMap`. language of verification looks like:
- `line=<uint> lvalue=<str>`
e.g., line=12 lvalue=s
e.g., line=12 lvalue=s.x
test arguments are written in one line and can be put in any order.
there must not be spaces anywhere else
*/
class LineNumberToLValueTester {
  public:
    static bool runTest(const std::string& testName, LineNumberToLValueMap& lineNumberToLValueMap);
};

#endif