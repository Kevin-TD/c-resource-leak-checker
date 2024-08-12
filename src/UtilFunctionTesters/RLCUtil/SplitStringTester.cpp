#include "UtilFunctionTesters/RLCUtil/SplitStringTester.h"
#include "Utils.h"

SplitStringTester::SplitStringTester() {}

bool testSplitString(std::string input, char delimeter, unsigned expectedSize, std::vector<std::string> expectedSplit) {
    auto result = rlc_util::splitString(input, delimeter);

    if (result.size() != expectedSize) {
        return false;
    }

    for (unsigned i = 0; i < expectedSize; i++) {
        if (result[i] != expectedSplit[i]) {
            return false;
        }
    }

    return true;
}

bool SplitStringTester::runTest() {

    return testSplitString("one two", ' ', 2, {"one", "two"}) &&
           testSplitString("tttt", 't', 4, {"", "", "", ""}) &&
           testSplitString("test", '\'', 1, {"test"}) &&
           testSplitString("", ' ', 0, {});

}