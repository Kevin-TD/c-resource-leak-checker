#include "LineNumberToLValueTester.h"
#include "Constants.h"
#include "Utils.h"
#include "Debug.h"

bool LineNumberToLValueTester::runTest(const std::string &testName, LineNumberToLValueMap &lineNumberToLValueMap) {
    bool testPassed = EXIT_SUCCESS;
    std::ifstream testFile(LINE_NUMBER_TO_L_VALUE_TEST_DIRNAME + "/" + testName + ".txt");
    std::string line;

    if (testFile.is_open()) {
        while (std::getline(testFile, line)) {
            // test arguments are written in one line and can be put in any order.
            // they look like the following:
            // line=<uint> lvalue=<str>
            // e.g., line=12 lvalue=s
            // e.g., line=12 lvalue=s.x
            // there must not be spaces anywhere else

            unsigned lineNumber;
            std::string lvalue;

            std::vector<std::string> arguments = rlc_util::splitString(line, ' ');
            for (std::string arg : arguments) {
                std::vector<std::string> argChunks = rlc_util::splitString(arg, '=');

                if (argChunks.size() != 2) {
                    logout("**LINE NUMBER TO L-VALUE TESTER ERROR: Too few or too many spaces in argument '" << arg << "' on line '" << line << "'.");
                    std::exit(EXIT_FAILURE);
                }

                std::string type = argChunks[0];
                std::string input = argChunks[1];

                if (type == "line") {
                    lineNumber = std::stoi(input);
                } else if (type == "lvalue") {
                    lvalue = input;
                } else {
                    logout("**LINE NUMBER TO L-VALUE TESTER ERROR: Unrecognized argument '" << type << "' on line '" << line << "'");
                    std::exit(EXIT_FAILURE);
                }
            }

            if (lvalue == "") {
                logout("**LINE NUMBER TO L-VALUE TESTER ERROR: L-value argument missing on line '" << line << "'");
                std::exit(EXIT_FAILURE);
            }

            if (!lineNumberToLValueMap.lineNumberIsInMap(lineNumber)) {
                logout("**LINE NUMBER TO L-VALUE TESTER ERROR: Did not find l-value for line number " << lineNumber);
                testPassed = EXIT_FAILURE;
            } else {
                if (lineNumberToLValueMap.get(lineNumber) != lvalue) {
                    logout("**LINE NUMBER TO L-VALUE TESTER ERROR: Expected '" << lvalue << "' on line " << lineNumber << " but received '" << lineNumberToLValueMap.get(lineNumber) << "'");
                    testPassed = EXIT_FAILURE;
                }
            }
        }
    }

    testFile.close();

    return testPassed;
}