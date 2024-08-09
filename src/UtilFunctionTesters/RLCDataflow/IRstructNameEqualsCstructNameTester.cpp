#include "UtilFunctionTesters/RLCDataflow/IRstructNameEqualsCstructNameTester.h"
#include "Utils.h"

IRstructNameEqualsCstructNameTester::IRstructNameEqualsCstructNameTester() {}

bool IRstructNameEqualsCstructNameTester::runTest() {

    std::string structName1 = "my_struct";
    std::string structName2 = "DNE";
    std::string cFileToTest = "../test/call_all_small_test/call_2.c";
    
    if (!rlc_dataflow::IRstructNameEqualsCstructName(structName1, cFileToTest)) {
        return false;
    }

    if (rlc_dataflow::IRstructNameEqualsCstructName(structName2, cFileToTest)) {
        return false;
    }


    return true;
}