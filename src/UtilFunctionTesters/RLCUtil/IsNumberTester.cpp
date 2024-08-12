#include "UtilFunctionTesters/RLCUtil/IsNumberTester.h"
#include "Utils.h"

IsNumberTester::IsNumberTester() {}

bool IsNumberTester::runTest() {
    std::map<std::string, bool> expectedResultMap;
    expectedResultMap["1"] = true;
    expectedResultMap["1 "] = false;
    expectedResultMap[""] = false;
    expectedResultMap["1234567890123456789012345678901234567890"] = true;

    for (auto pair : expectedResultMap) {
        auto testString = pair.first;
        auto expectedResult = pair.second;

        if (expectedResult != rlc_util::isNumber(testString)) {
            return false;
        }
    }

    return true;
}