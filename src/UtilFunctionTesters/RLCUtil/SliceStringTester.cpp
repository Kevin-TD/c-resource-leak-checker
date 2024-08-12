#include "UtilFunctionTesters/RLCUtil/SliceStringTester.h"
#include "Utils.h"

SliceStringTester::SliceStringTester() {}

bool SliceStringTester::runTest() {
    return rlc_util::sliceString("str", 0, 1) == "st" &&
           rlc_util::sliceString("str", 0, 500) == "str" &&
           rlc_util::sliceString("str", 0, 0) == "s";
}