#include "BranchListerTester.h"
#include "Constants.h"
#include "Utils.h"
#include "Debug.h"

bool BranchListerTester::runTest(const std::string& testName, ProgramLinesBranchInfo& programLinesBranchInfo) {
    bool testPassed = EXIT_SUCCESS;
    std::ifstream testFile(BRANCH_LISTER_TEST_DIRNAME + "/" + testName + ".txt");
    std::string line;

    if (testFile.is_open()) {
        while (std::getline(testFile, line)) {
            // test arguments are written in one line and can be put in any order.
            // they look like the following:
            // function=<str> line=<unsigned> branch=<str> preds=<set<str>>
            // e.g., function=main line=12 branch=main preds={}
            // there must not be any spaces anywhere else

            std::string functionName, branchName;
            unsigned lineNumber;
            std::set<std::string> predsSet;

            std::vector<std::string> arguments = rlc_util::splitString(line, ' ');
            for (std::string arg : arguments) {
                std::vector<std::string> argChunks = rlc_util::splitString(arg, '=');

                if (argChunks.size() != 2) {
                    logout("**BRANCH LISTER TESTER ERROR: Too few or too many spaces in argument '" << arg << "' on line '" << line << "'. ");
                    std::exit(EXIT_FAILURE);
                }

                std::string type = argChunks[0];
                std::string input = argChunks[1];

                if (type == "function") {
                    functionName = input;
                } else if (type == "line") {

                    if (!rlc_util::isNumber(input)) {
                        logout("**BRANCH LISTER TESTER ERROR: input '" << arg << "' on line '" << line << "' must be a number");
                        std::exit(EXIT_FAILURE);
                    }

                    lineNumber = std::stoi(input);
                } else if (type == "branch") {
                    branchName = input;
                } else if (type == "preds") {
                    std::vector<std::string> predsSetVec = rlc_util::splitString(
                            rlc_util::sliceString(input, input.find('{') + 1,
                                                  input.find('}') - 1),
                            ',');
                    predsSet =
                        std::set<std::string>(predsSetVec.begin(), predsSetVec.end());
                } else {
                    logout("**BRANCH LISTER TESTER ERROR: Unrecognized argument '"
                           << type << "' on line '" << line << "'");
                    std::exit(EXIT_FAILURE);
                }
            }

            if (functionName == "") {
                logout("**BRANCH LISTER TESTER ERROR: Function name argument missing on line '"
                       << line << "'");
                std::exit(EXIT_FAILURE);
            }

            if (branchName == "") {
                logout("**BRANCH LISTER TESTER ERROR: Branch name argument missing on line '"
                       << line << "'");
                std::exit(EXIT_FAILURE);
            }

            if (auto lbi = programLinesBranchInfo.getLBI(lineNumber)) {
                bool expectedEqualsGeneratedLBI = lbi->getFunctionName() == functionName &&
                                                  lbi->getBranchName() == branchName &&
                                                  lbi->getPreds() == predsSet;

                if (!expectedEqualsGeneratedLBI) {
                    auto lbiPreds = lbi->getPreds();
                    logout("**BRANCH LISTER TESTER ERROR - MISMATCH FOR LINE " << lineNumber);
                    logout("EXPECTED function=" << functionName << " branch=" << branchName << " preds=" << rlc_util::setToString(predsSet));
                    logout("RECEIVED function=" << lbi->getFunctionName() << " branch=" << lbi->getBranchName() << " preds=" << rlc_util::setToString(lbiPreds));
                    testPassed = EXIT_FAILURE;
                }
            } else {
                logout("**BRANCH LISTER TESTER ERROR: Did not find branch information for line " << lineNumber);
                testPassed = EXIT_FAILURE;
            }
        }
    }

    testFile.close();

    return testPassed;
}