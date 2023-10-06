#include "TestRunner.h"
#include "Debug.h"
#include "Utils.h"

bool TestRunner::runTests(const std::string functionName,
                          const std::string lastBranchName,
                          FullFile expectedResult,
                          ProgramFunction receivedResult) {
  bool testPassed = EXIT_SUCCESS;

  logout("Function Name Test Running = " << functionName)

      ProgramFunction function =
          expectedResult.getProgramFunction(functionName, true);
  std::list<ProgramPoint> points = function.getProgramPoints();

  for (ProgramPoint point : points) {
    std::string branchName = point.getPointName();

    if (branchName == "") {
      branchName = lastBranchName;
    }

    logout("branch = " << branchName)

        std::list<ProgramVariable>
            vars = point.getProgramVariables();
    for (ProgramVariable var : vars) {
      std::string varName = var.getCleanedName();

      MethodsSet expectedMethodsSet = var.getMethodsSet();
      MethodsSet receivedMethodsSet =
          receivedResult.getProgramPointRef(branchName, true)
              ->getPVRef(varName, true)
              ->getMethodsSet();

      std::set<std::string> expectedSet = expectedMethodsSet.getMethods();
      std::string expectedSetString = dataflow::setToString(expectedSet);

      std::set<std::string> receivedSet = receivedMethodsSet.getMethods();
      std::string receivedSetString = dataflow::setToString(receivedSet);

      errs() << "Test for branch name = " << branchName
             << " var name = " << varName;

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
        expectedResult.getProgramFunctionRef(functionName, true)
            ->getProgramPointRef(branchName, true)
            ->getPVRef(varName, true)
            ->setMethodsSet(methodsSet);
      }
    }
  }

  testFile.close();
  return expectedResult;
}