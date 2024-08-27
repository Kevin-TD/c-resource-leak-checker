#include "FunctionInfoTester.h"
#include "Constants.h"
#include "Utils.h"
#include "Debug.h"

bool FunctionInfoTester::runTest(const std::string& testName, FunctionInfosManager& functionInfosManager) {
    bool testPassed = EXIT_SUCCESS;
    std::ifstream testFile(FUNCTION_INFO_TEST_DIRNAME + "/" + testName + ".txt");
    std::string line;

    if (testFile.is_open()) {
        while (std::getline(testFile, line)) {
            // test arguments are written in one line and can be put in any order.
            // they look like the following:
            // function=<str>|return=<str>|params=<set<str>>
            // e.g., function=malloc0|return=void*|params={size_t,int}.
            // if the parameter or return type is a struct,
            // it must be specified with "struct <struct_name>"
            // (because the function info pass may return some struct M as either
            // "M" or "struct M")

            std::string functionName, returnType;
            std::vector<std::string> params;

            std::vector<std::string> arguments = rlc_util::splitString(line, '|');
            for (std::string arg : arguments) {
                std::vector<std::string> argChunks = rlc_util::splitString(arg, '=');

                if (argChunks.size() != 2) {
                    logout("**FUNCTION INFO TESTER ERROR: Too few or too many pipes (|) in argument '" << arg << "' on line '" << line << "'");
                    std::exit(EXIT_FAILURE);
                }

                std::string type = argChunks[0];
                std::string input = argChunks[1];

                if (type == "function") {
                    functionName = input;
                } else if (type == "return") {
                    returnType = input;
                } else if (type == "params") {
                    params = rlc_util::splitString(
                                 rlc_util::sliceString(input, input.find('{') + 1,
                                                       input.find('}') - 1),
                                 ',');
                } else {
                    logout("**FUNCTION INFO TESTER ERROR: Unrecognized argument '" << type << "' on line '" << line << "'");
                    std::exit(EXIT_FAILURE);
                }
            }

            if (functionName == "") {
                logout("**FUNCTION INFO TESTER ERROR: Function name argument missing on line '" << line << "'");
                std::exit(EXIT_FAILURE);
            }

            if (returnType == "") {
                logout("**FUNCTION INFO TESTER ERROR: Return type argument missing on line '" << line << "'");
                std::exit(EXIT_FAILURE);
            }

            if (auto receivedFunction = functionInfosManager.getFunction(functionName)) {

                if (receivedFunction->getReturnType() != returnType &&
                        "struct " + receivedFunction->getReturnType() != returnType) {
                    logout("**FUNCTION INFO TESTER ERROR: expected return type '" << returnType << "' but received '"  << receivedFunction->getReturnType() << "'");
                    testPassed = EXIT_FAILURE;
                }

                if (receivedFunction->getNumberOfParameters() != params.size()) {
                    logout("**FUNCTION INFO TESTER ERROR: Received number of params not equal to expected number of params for function '" << functionName << "'");
                } else {
                    for (unsigned i = 0; i < receivedFunction->getNumberOfParameters(); i++) {
                        if (receivedFunction->getNthParamType(i) != params[i] &&
                                "struct " + receivedFunction->getNthParamType(i) != params[i]) {
                            logout("**FUNCTION INFO TESTER ERROR: Parameter " << i << " mismatch: expected '" << params[i] << "' but received '" << receivedFunction->getNthParamType(i) << "'");
                            testPassed = EXIT_FAILURE;
                        }
                    }
                }
            } else {
                logout("**FUNCTION INFO TESTER ERROR: Did not find function info for function '" << functionName << "'");
                testPassed = EXIT_FAILURE;
            }
        }
    }

    testFile.close();

    return testPassed;
}