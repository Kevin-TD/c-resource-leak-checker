#include "UtilFunctionTesters/RLCUtil/StartsWithTester.h"
#include "Utils.h"

StartsWithTester::StartsWithTester() {}

bool StartsWithTester::runTest() {
    return rlc_util::startsWith("test", "t") == true &&
           rlc_util::startsWith("test", "te") == true &&
           rlc_util::startsWith("test", "tes") == true &&
           rlc_util::startsWith("test", "test") == true &&
           rlc_util::startsWith("test", "1") == false &&
           rlc_util::startsWith("test", "") == true &&
           rlc_util::startsWith("", "") == true &&
           rlc_util::startsWith("1", "test") == false;
}