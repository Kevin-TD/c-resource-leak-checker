#include "TestRunner.h"
#include "Debug.h"

bool TestRunner::runTests(MappedMethods expectedResult, MappedMethods receivedResult) {
    bool testPassed = EXIT_SUCCESS; 

   for (auto Pair1 : expectedResult) {
    std::string branchName = Pair1.first; 
    logout("branch = " << branchName)
    
    for (auto Pair2 : Pair1.second) {

      std::string pass; 
      std::string varName =  Pair2.first; 

      int itr = 0; 
      for (auto passString : Pair2.second.methodsSet) {
        if (itr  != Pair2.second.methodsSet.size() - 1) {
          pass += passString + ", ";
        }
        else {
          pass += passString; 
        }

        itr++; 
      }

      std::set<std::string> analysisPassSet = receivedResult[branchName][varName].methodsSet;
      std::set<std::string> expectedPassSet = Pair2.second.methodsSet; 

      std::string analysisPass; 
      itr = 0; 
      for (auto passString : analysisPassSet) {
        if (itr != analysisPassSet.size() - 1) {
          analysisPass += passString + ", ";
        }
        else {
          analysisPass += passString; 
        }

        itr++; 
      }


      errs() << "Test for branch name = " << branchName << " var name = " << varName;

      if (analysisPassSet == expectedPassSet) {
        errs() << " passed\n"; 
      }
      else {
        errs() << " **FAILED**\n"; 
        testPassed = EXIT_FAILURE; 


      }
      errs() << "EXPECTED {" << pass << "}\n"; 
      errs() << "RECEIVED {" << analysisPass << "}\n\n"; 


    }
  }

  return testPassed; 
}