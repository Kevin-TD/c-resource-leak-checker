#include "UtilFunctionTesters/RLCUtil/SetToStringTester.h"

SetToStringTester::SetToStringTester() {}

bool SetToStringTester::runTest() {
    std::set<std::string> set1 = {"test"};
    std::set<std::string> set2 = {"test1", "test2"};
    std::set<std::string> set3 = {""};
    std::set<std::string> set4 = {"", ""};

    return rlc_util::setToString(set1) == "{test}" &&
           rlc_util::setToString(set2) == "{test1, test2}" &&
           rlc_util::setToString(set3) == "{}" &&
           rlc_util::setToString(set4) == "{}";

}