#include "UtilFunctionTesters/RLCUtil/RemoveWhitespaceTester.h"

RemoveWhitespaceTester::RemoveWhitespaceTester() {}

bool RemoveWhitespaceTester::runTest() {
    std::vector<std::string> testStrings = {
        "x",
        "x, y, z",
        "x,y",
        "x, y,z"
    };
    std::vector<std::string> expectedOutputs = {
        "x",
        "x,y,z",
        "x,y",
        "x,y,z"
    };

    if (testStrings.size() != expectedOutputs.size()) {
        errs() << "Remove whitespace test build error: list of test string size not equal to list of expected output strings\n";
        std::exit(EXIT_FAILURE);
    }

    for (auto& testString : testStrings) {
        rlc_util::removeWhitespace(testString);
    }

    for (unsigned i = 0; i < testStrings.size(); i++) {
        if (testStrings[i] != expectedOutputs[i]) {
            return false;
        }
    }

    return true;
}
