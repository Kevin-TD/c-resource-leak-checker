#include "PassType.h"
#include <fstream>

#define DEBUG true
#if DEBUG
#define logout(x) errs() << x << "\n";
#define logDomain(x) x->print(errs()); 
#define logOutMemory(x) printMemory(x);
#else
#define logout(x) 
#define logDomain(x) 
#define logOutMemory(x) 
#endif 

void PassType::setFunctions(std::map<std::string, bool> safeFunctions, std::map<std::string, bool> unsafeFunctions, std::map<std::string, bool> reallocFunctions, std::map<std::string, std::string> memoryFunctions) {
    this->safeFunctions = safeFunctions;
    this->unsafeFunctions = unsafeFunctions; 
    this->reallocFunctions = reallocFunctions; 
    this->memoryFunctions = memoryFunctions; 
}

void PassType::buildExpectedResult(std::string testName) {
  std::ifstream testFile("../test/" + testName + ".txt");
  std::string line; 

  if (testFile.is_open()) {
    while (std::getline(testFile, line)) {
      if (line.size() > 14 && line.substr(0, 14) == "ALLOW_REDEFINE") {
        continue; 
      }

      int spaceCount = 0; 
      std::string branchName;
      std::string varName;
      std::set<std::string> calledMethodsSet; 
      std::string currentCM; 
      std::string passType; 

      for (char c : line) {
        if (c == ' ') {
          spaceCount++;
          continue; 
        }

        if (spaceCount == 0) {
          passType += c; 
        }
        else if (spaceCount == 1) {
          if (passType != this->passName) {
            break; 
          }
          
          branchName += c; 
        }
        else if (spaceCount == 2) {
          varName += c; 
        }
        else if (spaceCount == 3) {
          if (c == '{') {
            continue; 
          }
          if (c == ',' || c == '}') {
            if (currentCM.size() > 0)
              calledMethodsSet.insert(currentCM);
            currentCM = ""; 
            continue; 
          }
          currentCM += c; 


        }
      }

      if (passType == this->passName) {
        this->expectedResult[branchName][varName] = {
          calledMethodsSet, 
          true 
        }; 
      }
    }

  }
  testFile.close(); 
}

bool PassType::runTests(MappedMethods receivedResult) {
    bool testPassed = EXIT_SUCCESS; 

   for (auto Pair1 : this->expectedResult) {
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


MappedMethods PassType::generatePassResults() {
    MappedMethods PreMappedMethods; 
    MappedMethods PostMappedMethods; 
    this->analyzeCFG(this->cfg, PreMappedMethods, PostMappedMethods, ""); 
    return PostMappedMethods; 
}

void PassType::setCFG(CFG* cfg) {
    this->cfg = cfg; 
}

void PassType::setAliasedVars(AliasMap aliasedVars) {
    this->aliasedVars = aliasedVars; 
}

bool getAllowedRedefine(std::string testName) {
    std::ifstream testFile("../test/" + testName + ".txt");
    std::string line; 
    bool ALLOW_REDEFINE;

    if (testFile.is_open()) {
        while (std::getline(testFile, line)) {
            if (line.size() > 14 && line.substr(0, 14) == "ALLOW_REDEFINE") {
                std::string allowedRedefValue = line.substr(15);
                if (allowedRedefValue == "true") {
                    ALLOW_REDEFINE = true; 
                    break;
                }
                else if (allowedRedefValue == "false") {
                    ALLOW_REDEFINE = false; 
                    break; 
                }
            }
        }

    }
    testFile.close(); 

    return ALLOW_REDEFINE; 
}