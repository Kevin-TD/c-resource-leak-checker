#include "TestRunner.h"
#include "Debug.h"
#include "Utils.h"

bool TestRunner::runTests(const std::string functionName,
                          const std::string lastBranchName,
                          FunctionMappedMethods expectedResult,
                          MappedMethods receivedResult) {
  bool testPassed = EXIT_SUCCESS;

  logout("Function Name Test Running = "
         << functionName) for (auto Pair1 : expectedResult[functionName]) {
    std::string branchName = Pair1.first;
    if (branchName == "") {
      branchName = lastBranchName;
    }

    logout("branch = " << branchName)

        for (auto Pair2 : Pair1.second) {
      std::string pass = dataflow::setToString(Pair2.second.methodsSet);
      std::string varName = Pair2.first;

      std::set<std::string> analysisPassSet =
          receivedResult[branchName][varName].methodsSet;
      std::set<std::string> expectedPassSet = Pair2.second.methodsSet;

      std::string analysisPass = dataflow::setToString(analysisPassSet);

      errs() << "Test for branch name = " << branchName
             << " var name = " << varName;

      if (analysisPassSet == expectedPassSet) {
        errs() << " passed\n";
      } else {
        errs() << " **FAILED**\n";
        testPassed = EXIT_FAILURE;
      }
      errs() << "EXPECTED " << pass << "\n";
      errs() << "RECEIVED " << analysisPass << "\n\n";
    }
  }

  return testPassed;
}

FunctionMappedMethods TestRunner::buildExpectedResults(std::string testName,
                                                       std::string passName) {
  std::ifstream testFile("../test/" + testName + ".txt");
  std::string line;
  FunctionMappedMethods expectedResult;

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

      std::vector<std::string> arguments = dataflow::splitString(line, ' ');
      for (std::string arg : arguments) {
        std::vector<std::string> argChunks = dataflow::splitString(arg, '=');

        if (argChunks.size() != 2) {
          logout("**TEST RUNNER ERROR: Too few or too many spaces in argument '"
                 << arg << "' on line '" << line
                 << "'. Make sure methods arg contains no spaces (e.g., {x, y} "
                    "is wrong, {x,y} is correct) Returning result "
                    "early.") return expectedResult;
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
          std::vector<std::string> methodsSetVec = dataflow::splitString(
              dataflow::sliceString(input, input.find('{') + 1,
                                    input.find('}') - 1),
              ',');
          methodsSet =
              std::set<std::string>(methodsSetVec.begin(), methodsSetVec.end());
        } else {
          logout("**TEST RUNNER ERROR: Unrecognized argument '"
                 << type << "' on line '" << line
                 << "'. Returning early") return expectedResult;
        }
      }

      if (functionName == "") {
        logout("**TEST RUNNER ERROR: Function name argument missing on line '"
               << line << "'. Returning early") return expectedResult;
      }

      if (varName == "") {
        logout("**TEST RUNNER ERROR: Variable name argument missing on line '"
               << line << "'. Returning early") return expectedResult;
      }

      if (passName == inputPassName) {
        expectedResult[functionName][branchName][varName] = {methodsSet, true};
      }
    }
  }

  testFile.close();
  return expectedResult;
}