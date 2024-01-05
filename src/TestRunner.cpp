#include "TestRunner.h"
#include "Debug.h"
#include "Utils.h"

bool TestRunner::runTests(const std::string functionName,
                          const std::string lastBranchName,
                          FullFile expectedResult,
                          ProgramFunction receivedResult, StructFieldToIndexMap structFieldToIndexMap) {
    bool testPassed = EXIT_SUCCESS;

    logout("Function Name Test Running = " << functionName);

    ProgramFunction function =
        expectedResult.getProgramFunction(functionName, true);
    std::list<ProgramPoint> points = function.getProgramPoints();

    for (ProgramPoint expectedResultPoint : points) {
        std::string branchName = expectedResultPoint.getPointName();

        if (branchName == "") {
            branchName = lastBranchName;
        }

        logout("branch = " << branchName);

        DisjointedPVAliasSets expectedDPVAS =
            expectedResultPoint.getProgramVariableAliasSets();

        ProgramPoint receivedResultPoint =
            receivedResult.getProgramPoint(branchName, true);

        for (PVAliasSet expctedPVAS : expectedDPVAS.getSets()) {
            std::set<std::string> expectedSet =
                expctedPVAS.getMethodsSet().getMethods();
            std::string expectedSetString = rlc_util::setToString(expectedSet);

            // TODO: change structure for expectedResult.
            // expctedPVAS.getProgramVariables().size() will always equal 1, so
            // this for loop is misleading
            for (ProgramVariable expectedPV : expctedPVAS.getProgramVariables()) {
                std::string expectedPVName = expectedPV.getCleanedName();
                std::string expectedPVRefName = expectedPVName;

                if (structFieldToIndexMap.structNameAndFieldIsInMap(expectedPVName)) {
                    expectedPVRefName = structFieldToIndexMap.get(expectedPVName);
                }

                PVAliasSet *receivedPVAS =
                    receivedResultPoint.getPVASRef(expectedPVRefName, true);

                std::set<std::string> receivedSet =
                    receivedPVAS->getMethodsSet().getMethods();
                std::string receivedSetString = rlc_util::setToString(receivedSet);

                errs() << "Test for branch name = " << branchName
                       << " var name = " << expectedPVName;

                if (expectedSet == receivedSet) {
                    errs() << " passed\n";
                } else {
                    errs() << " **FAILED**\n";
                    testPassed = EXIT_FAILURE;
                }
                errs() << "EXPECTED " << expectedSetString << "\n";
                errs() << "RECEIVED " << receivedSetString << "\n\n";
            }
        }
    }

    return testPassed;
}

FullFile TestRunner::buildExpectedResults(std::string testName,
        std::string passName) {

    std::ifstream testFile("../Testers/Passes/" + testName + ".txt");
    std::string line;
    FullFile expectedResult(testName);

    if (testFile.is_open()) {
        while (std::getline(testFile, line)) {
            // we will be assuming our test case writing is correctly formatted.
            // test arguments are written in one line and can be put in any order.
            // they look like the following:
            // pass=<str> function=<str> branch=<str> var=<str> methods=<set<str>>
            // e.g., pass=MustCall function=main branch=entry var=s methods={free}.
            // there must not be any spaces anywhere else, including in methods
            // argument. to add more methods to method arg, it would look like:
            // methods={x,y}.

            std::string inputPassName, branchName, varName, functionName;
            std::set<std::string> methodsSet;

            std::vector<std::string> arguments = rlc_util::splitString(line, ' ');
            for (std::string arg : arguments) {
                std::vector<std::string> argChunks = rlc_util::splitString(arg, '=');

                if (argChunks.size() != 2) {
                    logout("**TEST RUNNER ERROR: Too few or too many spaces in argument '"
                           << arg << "' on line '" << line
                           << "'. Make sure methods arg contains no spaces (e.g., {x, y} "
                           "is wrong, {x,y} is correct).");
                    std::exit(EXIT_FAILURE);
                }

                std::string type = argChunks[0];
                std::string input = argChunks[1];

                if (type == "pass") {
                    inputPassName = input;
                } else if (type == "function") {
                    functionName = input;
                } else if (type == "branch") {
                    branchName = input;
                } else if (type == "var") {
                    varName = input;
                } else if (type == "methods") {
                    std::vector<std::string> methodsSetVec = rlc_util::splitString(
                                rlc_util::sliceString(input, input.find('{') + 1,
                                                      input.find('}') - 1),
                                ',');
                    methodsSet =
                        std::set<std::string>(methodsSetVec.begin(), methodsSetVec.end());
                } else {
                    logout("**TEST RUNNER ERROR: Unrecognized argument '"
                           << type << "' on line '" << line << "'");
                    std::exit(EXIT_FAILURE);
                }
            }

            if (functionName == "") {
                logout("**TEST RUNNER ERROR: Function name argument missing on line '"
                       << line << "'");
                std::exit(EXIT_FAILURE);
            }

            if (varName == "") {
                logout("**TEST RUNNER ERROR: Variable name argument missing on line '"
                       << line << "'");
                std::exit(EXIT_FAILURE);
            }

            if (passName == inputPassName) {
                expectedResult.getProgramFunctionRef(functionName, true)
                ->getProgramPointRef(branchName, true)
                ->getPVASRef(varName, true)->methods = methodsSet;
            }
        }
    }

    testFile.close();
    return expectedResult;
}