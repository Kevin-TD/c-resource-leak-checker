#include "UtilFunctionTesters/RLCUtil/IsValidCVariableNameTester.h"
#include "Utils.h"

IsValidCVariableNameTester::IsValidCVariableNameTester() {}

bool IsValidCVariableNameTester::runTest() {
    return rlc_util::isValidCVariableName("test") == true &&
           rlc_util::isValidCVariableName("") == false &&
           rlc_util::isValidCVariableName("test test") == false &&
           rlc_util::isValidCVariableName("1test") == false;
    rlc_util::isValidCVariableName("Test") == true;
    rlc_util::isValidCVariableName("test1") == true;
    rlc_util::isValidCVariableName("te.st") == false;
}