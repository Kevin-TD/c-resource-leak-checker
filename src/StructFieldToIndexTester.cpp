#include "StructFieldToIndexTester.h"
#include "Constants.h"
#include "Utils.h"
#include "Debug.h"

bool StructFieldToIndexTester::runTest(const std::string &testName, StructFieldToIndexMap &structFieldToIndexMap) {
    bool testPassed = EXIT_SUCCESS;
    std::ifstream testFile(STRUCT_FIELD_TO_INDEX_TEST_DIRNAME + "/" + testName + ".txt");
    std::string line;
    std::map<std::string, std::string> expectedFieldToIndexMap;

    if (testFile.is_open()) {
        while (std::getline(testFile, line)) {
            // there should be one test argument per line that looks like
            // [variable with field name]=[variable with field index]
            // e.g., for struct M {x, y} and variable M m, there should be
            // m.x=m.0 and m.y=m.1

            std::vector<std::string> arguments = rlc_util::splitString(line, '=');

            if (arguments.size() != 2) {
                logout("**STRUCT FIELD TO INDEX TESTER ERROR: Line '" << line << "' does not look like key-pair map in the form 'a=b'");
                std::exit(EXIT_FAILURE);
            }

            std::string structNameAndField = arguments[0];
            std::string structNameAndIndex = arguments[1];

            expectedFieldToIndexMap[structNameAndField] = structNameAndIndex;

        }
    }

    for (auto pair : expectedFieldToIndexMap) {
        std::string expectedStructNameAndField = pair.first;
        std::string expectedStructNameAndIndex = pair.second;

        if (structFieldToIndexMap.get(expectedStructNameAndField) != expectedStructNameAndIndex) {
            logout("**STRUCT FIELD TO INDEX TESTER ERROR: for field '" << expectedStructNameAndField <<
                   "', expected '" << expectedStructNameAndIndex <<
                   "' and received '" << structFieldToIndexMap.get(expectedStructNameAndField) << "'");
            testPassed = EXIT_FAILURE;

        }
    }

    testFile.close();
    return testPassed;
}