#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "CFG.h"

#include <map> 
#include <tuple>
#include <vector> 
#include <list>
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

// to run: cd build   then 
// sh ../run_test.sh <test_num> 
// or to run all tests: sh ../run_all.sh 
// note for run all tests is that if you add more tests, you'll have to modify run_all.sh to include that test number

// updated: automated testing, aliasing, checking if inputs have changed 
// todo: must call pass; what is done for branching if in one branch malloc is called and another it is not? just need it explained

struct CM {
  std::set<std::string> cmSet; 
  bool setInitialized; 
};

struct InstructionHolder {
    SetVector<Instruction*> branch; 
    SetVector<Instruction*> successors; 
}; 

typedef std::map<std::string, std::map<std::string, CM>> CalledMethods; // left = branch name. inner left = var name 
typedef std::map<std::string, std::string> AliasMap;
typedef std::map<std::string, std::string> VarBranchMap;


namespace dataflow {

std::map<std::string, bool> SafeFunctions;
std::map<std::string, bool> UnsafeFunctions;
std::map<std::string, bool> ReallocFunctions;
std::map<std::string, std::string> MemoryFunctions;
std::vector<std::string> realBranchOrder; 
CalledMethods ExpectedResult; 
bool ALLOW_REDEFINE;  // ONLY for debugging purposes. exists so we can make our own functions without code saying it is a re-definition or clang saying it is undefined 

void loadFunctions() { 
  // working directory is /build 

  std::ifstream safeFunctionsFile("../src/Functions/safe.txt");
  std::ifstream unsafeFunctionsFile("../src/Functions/unsafe.txt");
  std::ifstream reallocFunctionsFile("../src/Functions/realloc.txt");
  std::ifstream memoryFunctionsFile("../src/Functions/test_memory.txt"); //*NOTE: test memory being used here instead of "real" memory allocation functions 

  std::string line; 
  if (safeFunctionsFile.is_open()) {
    while (std::getline(safeFunctionsFile, line)) {
      SafeFunctions[line] = true; 
    }
  }

  if (unsafeFunctionsFile.is_open()) {
    while (std::getline(unsafeFunctionsFile, line)) {
      UnsafeFunctions[line] = true; 
    }
  }

  if (reallocFunctionsFile.is_open()) {
    while (std::getline(reallocFunctionsFile, line)) {
      ReallocFunctions[line] = true; 
    }
  }

  if (memoryFunctionsFile.is_open()) {
    while (std::getline(memoryFunctionsFile, line)) {
      std::string allocationFunction;
      std::string deallocationFunction;  
      std::string s;          

      for (int i = 0; i < line.size(); i++) {
        if (line[i] == ' ') {
          allocationFunction = s; 
          s = ""; 
          continue;
        }
        s += line[i];
      }
      deallocationFunction = s; 
      
      MemoryFunctions[allocationFunction] = deallocationFunction;
    }

  }


  safeFunctionsFile.close();
  unsafeFunctionsFile.close();
  memoryFunctionsFile.close();
  reallocFunctionsFile.close();


}

std::string getTestName(std::string optLoadFileName) {
  int slashCount = 0; 
  std::string testName; 
  // assuming that format of optLoadFileName is ../test/test{num}.c, returns test{num}
  for (char c : optLoadFileName) { 
    if (c == '/') {
      slashCount++; 
      continue; 
    }

    if (slashCount == 2) {
      if (c == '.') break;

      testName += c; 
    }
  }
  return testName; 
}

void loadExpectedResult(std::string testName, CalledMethods& expectedResult) {
  std::ifstream testFile("../test/" + testName + ".txt");
  std::string line; 

  if (testFile.is_open()) {
    while (std::getline(testFile, line)) {
      if (line.size() > 14 && line.substr(0, 14) == "ALLOW_REDEFINE") {
        std::string allowedRedefValue = line.substr(15);
        if (allowedRedefValue == "true") {
          ALLOW_REDEFINE = true; 
        }
        else if (allowedRedefValue == "false") {
          ALLOW_REDEFINE = false; 
        }
        continue; 
      }


      int spaceCount = 0; 
      std::string branchName;
      std::string varName;
      std::set<std::string> calledMethodsSet; 
      std::string currentCM; 

      for (char c : line) {
        if (c == ' ') {
          spaceCount++;
          continue; 
        }

        if (spaceCount == 0) {
          branchName += c; 
        }
        else if (spaceCount == 1) {
          varName += c; 
        }
        else if (spaceCount == 2) {
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

      expectedResult[branchName][varName] = {
        calledMethodsSet, 
        true 
      }; 
    }

  }
  testFile.close(); 
}


void buildCFG(CFG& topCFG, std::vector<std::string> branchOrder, std::map<std::string, InstructionHolder> branchInstMap) {
  topCFG = CFG("entry"); 
  std::map<std::string, CFG*> cfgMap; 
  cfgMap["entry"] = &topCFG; 

  for (auto branchName : branchOrder) {  
    auto succs = branchInstMap[branchName].successors; 

    CFG* cfg = cfgMap[branchName]; 

    cfg->setInstructions(branchInstMap[branchName].branch);

   for (auto succ : succs) {
    std::string succName = succ->getParent()->getName().str();

    if (succName == branchName) continue; 

    if (cfgMap.count(succName) > 0) {
      cfg->addSuccessor(cfgMap[succName]);
      continue; 
    } 


    cfgMap[succName] = cfg->addSuccessor(succName); 



   }

  }
}

bool runTests(CalledMethods expectedCM, CalledMethods receivedCM) {
   bool testPassed = EXIT_SUCCESS; 

   for (auto Pair1 : expectedCM) {
    std::string branchName = Pair1.first; 
    logout("branch = " << branchName)
    
    for (auto Pair2 : Pair1.second) {

      std::string cm; 
      std::string varName =  Pair2.first; 

      int itr = 0; 
      for (auto cmString : Pair2.second.cmSet) {
        if (itr  != Pair2.second.cmSet.size() - 1) {
          cm += cmString + ", ";
        }
        else {
          cm += cmString; 
        }

        itr++; 
      }

     

      std::set<std::string> analysisCMSet = receivedCM[branchName][varName].cmSet;
      std::set<std::string> expectedCMSet = Pair2.second.cmSet; 


      std::string analysisCm; 
      itr = 0; 
      for (auto cmString : analysisCMSet) {
        if (itr != analysisCMSet.size() - 1) {
          analysisCm += cmString + ", ";
        }
        else {
          analysisCm += cmString; 
        }

        itr++; 
      }


      errs() << "Test for branch name = " << branchName << " var name = " << varName;

      if (analysisCMSet == expectedCMSet) {
        errs() << " passed\n"; 
      }
      else {
        errs() << " **FAILED**\n"; 
        testPassed = EXIT_FAILURE; 


      }
      errs() << "EXPECTED {" << cm << "}\n"; 
      errs() << "RECEIVED {" << analysisCm << "}\n\n"; 


    }
  }

  return testPassed; 
}

/**
 * @brief Get the Predecessors of a given instruction in the control-flow graph.
 *
 * @param Inst The instruction to get the predecessors of.
 * @return Vector of all predecessors of Inst.
 */
std::vector<Instruction *> getPredecessors(Instruction *Inst) {
  std::vector<Instruction *> Ret;
  auto Block = Inst->getParent();
  for (auto Iter = Block->rbegin(), End = Block->rend(); Iter != End; ++Iter) {
    if (&(*Iter) == Inst) {
      ++Iter;
      if (Iter != End) {
        Ret.push_back(&(*Iter));
        return Ret;
      }
      for (auto Pre = pred_begin(Block), BE = pred_end(Block); Pre != BE;
           ++Pre) {
        Ret.push_back(&(*((*Pre)->rbegin())));
      }
      return Ret;
    }
  }
  return Ret;
}

/**
 * @brief Get the successors of a given instruction in the control-flow graph.
 *
 * @param Inst The instruction to get the successors of.
 * @return Vector of all successors of Inst.
 */
std::vector<Instruction *> getSuccessors(Instruction *Inst) {
  std::vector<Instruction *> Ret;
  auto Block = Inst->getParent();
  for (auto Iter = Block->begin(), End = Block->end(); Iter != End; ++Iter) {
    if (&(*Iter) == Inst) {
      ++Iter;
      if (Iter != End) {
        Ret.push_back(&(*Iter));
        return Ret;
      }
      for (auto Succ = succ_begin(Block), BS = succ_end(Block); Succ != BS;
           ++Succ) {
        Ret.push_back(&(*((*Succ)->begin())));
      }
      return Ret;
    }
  }
  return Ret;
}

bool isNumber(const std::string& s) {
    for (char const &ch : s) {
        if (std::isdigit(ch) == 0) 
            return false;
    }
    return true;
 }
 

void populateAliasedVars(Instruction* I, SetVector<Instruction *>& workSet, AliasMap& aliasedVars) {
  bool includes = false; 
  std::string branchName = I->getParent()->getName().str();
  for (auto branch : realBranchOrder) {
    if (branch == branchName) {
      includes = true; 
      break;
    }
  }
  if (!includes) {
    realBranchOrder.push_back(branchName);
  }



 if (auto Load = dyn_cast<LoadInst>(I)) {
    logout("(load) name is " << variable(Load) << " for " << variable(Load->getPointerOperand()) )

      std::string varName = variable(Load->getPointerOperand()); 
      while (varName.size() > 1 && isNumber(varName.substr(1))) {
        varName = aliasedVars[varName]; 
      }

      logout(variable(Load) << " -> " << varName)

      aliasedVars[variable(Load)] = varName;
  }



}


void transfer(Instruction* I, SetVector<Instruction *>& workSet, std::map<std::string, CM>& calledMethodsSet, AliasMap& aliasedVars) {

  if (auto Alloca = dyn_cast<AllocaInst>(I)) {
    calledMethodsSet["%" + Alloca->getName().str()] = {};
    
  }
  else if (auto Load = dyn_cast<LoadInst>(I)) {
    logout("(load) name is " << variable(Load) << " for " << variable(Load->getPointerOperand()) )

      std::string varName = variable(Load->getPointerOperand()); 
      while (varName.size() > 1 && isNumber(varName.substr(1))) {
        varName = aliasedVars[varName]; 
      }

      logout(variable(Load) << " -> " << varName)

      aliasedVars[variable(Load)] = varName;
  }
  else if (auto Store = dyn_cast<StoreInst>(I)) {

    Value* valueToStore = Store->getOperand(0);      
    Value* receivingValue = Store->getOperand(1);   
 
    for (auto Inst : workSet) {
      if (valueToStore == Inst) {
        if (auto Call = dyn_cast<CallInst>(Inst)) {
          std::string varName = variable(Store->getOperand(1)); 
          std::string fnName = Call->getCalledFunction()->getName().str(); 
          if (
            UnsafeFunctions.count(fnName) == 0  && 
            UnsafeFunctions.count(MemoryFunctions[fnName]) == 0 && 
            MemoryFunctions[fnName].size() > 0
          ) {
            //  calledMethodsEst[branchName][varName].insert(fnName);   
            //  allocation function 

          }
          return; 
        }

      }
    }
    
    // for pointer aliasing
    std::string lval = variable(receivingValue); 
    std::string rval = variable(valueToStore);  

    if (lval[0] == rval[0] && lval[0] == '%') {
      aliasedVars[lval] = rval; 
    }
  

  
  }
  else if (auto Call = dyn_cast<CallInst>(I)) {
    for (unsigned i = 0; i < Call->getNumArgOperands(); ++i) {
        Value *argument = Call->getArgOperand(i);
        std::string argName = variable(argument);

        argName = aliasedVars[argName];
        while (argName.size() > 1 && isNumber(argName.substr(1))) {
          argName = aliasedVars[argName]; 
        }

        

        // * i THINK this is what distinguishes "real" variables defined in the program from constants and other irrelevant llvm stuff 
        if (argName[0] != '%') return; 

        std::list<std::string> allAliases;
        allAliases.push_back(argName); 
        while (argName != "") {
          argName = aliasedVars[argName]; 
          if (argName != "") 
            allAliases.push_back(argName); 
        }

        for (auto alias : allAliases) {
          logout("alias = " << alias)
        }


        // * (i think this handles) cases where an entirely random and undefined (implicitly declared) function shows up 
        if (Call->getCalledFunction() == NULL) {
          const DebugLoc &debugLoc = I->getDebugLoc();
          std::string location = "Line " + std::to_string(debugLoc.getLine()) + ", Col " + std::to_string(debugLoc.getCol());

          errs() << "CME-WARNING: unknown (unforseen occurence) & unsafe func on " << location << ". must call set for '" << argName << "' set to empty.\n";

          for (std::string aliasArg : allAliases) {
            calledMethodsSet[aliasArg].cmSet = {}; 
          }
          
          return; 
        }

        std::string fnName = Call->getCalledFunction()->getName().str();


        if (UnsafeFunctions[fnName]) {
          logout("unsafe func, cm set emptied")
          for (std::string aliasArg : allAliases) {
            calledMethodsSet[aliasArg].cmSet = {}; 
          }

          return; 
        }

        if (ReallocFunctions[fnName]) {
          logout("realloc func, cm set emptied")
          for (std::string aliasArg : allAliases) {
            calledMethodsSet[aliasArg].cmSet = {}; 
          }

          return; 
        }

        if (SafeFunctions[fnName]) {
          logout("safe func")
          return; 
        }


        for (auto Pair : MemoryFunctions) {
          if (fnName == Pair.first) { // allocation function

          }
          else if (fnName == Pair.second) { // deallocation function 
            for (std::string aliasArg : allAliases) {
              calledMethodsSet[aliasArg].cmSet.insert(fnName); 
              calledMethodsSet[aliasArg].setInitialized = true; 
            }
            
            break; 

          }
        }
        



    }
  }
  else if (auto Branch = dyn_cast<BranchInst>(I)) {

  }


}

void analyzeCFG(CFG* cfg, CalledMethods& PreCalledMethods, CalledMethods& PostCalledMethods, AliasMap& AliasedVars, std::string priorBranch) {
  std::string currentBranch =  cfg->getBranchName(); 

  if (currentBranch == "entry") { 

    PreCalledMethods[currentBranch] = {}; 
    llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 

    for (Instruction* I : instructions) {
      transfer(I, instructions, PostCalledMethods[currentBranch], AliasedVars);
    }


    for (CFG* succ : cfg->getSuccessors()) {
      analyzeCFG(succ, PreCalledMethods, PostCalledMethods, AliasedVars, currentBranch); 
    }

  }

  else {
    std::map<std::string, CM> PriorPreCM = std::map<std::string, CM>(PreCalledMethods[currentBranch]); 
    std::map<std::string, CM> PriorPostCM = std::map<std::string, CM>(PostCalledMethods[currentBranch]); 


    logout("-----\nlogging priorprecm prior = " << priorBranch << " current = " << currentBranch )
    for (auto p : PriorPreCM) {
      std::string m; 
      for (auto k : p.second.cmSet) {
        m += k + ", ";
      }
      logout(p.first << " " << m)
    }


    if (PriorPreCM.size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch)

      std::map<std::string, CM> CurrentPreCM = std::map<std::string, CM>(PostCalledMethods[priorBranch]);
      

      // check if inputs (pre) differ
      bool currentAndPriorPreCMEqual = true; 
      for (auto Pair : CurrentPreCM) {
        std::string varName = Pair.first; 
        CM currentPreCM = Pair.second; 

        if (
          (currentPreCM.cmSet != PriorPreCM[varName].cmSet) || 
          (currentPreCM.setInitialized != PriorPreCM[varName].setInitialized)
        ) {
          currentAndPriorPreCMEqual = false; 
          break; 
        }
      }

      for (auto Pair : PriorPreCM) {
        std::string varName = Pair.first; 
        CM priorPreCM = Pair.second; 

        if (
          (priorPreCM.cmSet != CurrentPreCM[varName].cmSet) || 
          (priorPreCM.setInitialized != CurrentPreCM[varName].setInitialized)
        ) {
          currentAndPriorPreCMEqual = false; 
          break; 
        }

      }

      if (currentAndPriorPreCMEqual) return; 

      llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 
    
      // lub PriorPreCM and CurrentPreCM
      std::map<std::string, CM> lub; 

      for (auto Pair1 : PriorPreCM) {
        std::string varName = Pair1.first; 
        CM priorPreCM = Pair1.second; 
        
          
        // intersection 
        std::set<std::string> intersection;

        std::set_intersection(priorPreCM.cmSet.begin(), priorPreCM.cmSet.end(), 
                      CurrentPreCM[varName].cmSet.begin(), CurrentPreCM[varName].cmSet.end(), 
                      std::inserter(intersection, intersection.begin()));

        logout("for var name " << varName)

        std::string m,k; 
        for (auto i : priorPreCM.cmSet) {
          m += i + ", ";
        }
        for (auto i : CurrentPreCM[varName].cmSet) {
          k += i + ", ";
        }
        logout("prior pre cm = " << m)
        logout("current pre cm = " << k)


        lub[varName] = {
          intersection, 
          true
        }; 
        
      
      }

    
      // fill in the rest 
      for (auto Pair1 : PriorPostCM) {
        std::string varName = Pair1.first; 
        CM priorPostCM = Pair1.second; 

        if (!lub[varName].setInitialized) {
          
          lub[varName] = {
            std::set<std::string>(priorPostCM.cmSet), 
            true 
          }; 

        }
      }

      PreCalledMethods[currentBranch] = std::map<std::string, CM>(lub);

      for (Instruction* I : instructions) {
        transfer(I, instructions, lub, AliasedVars);
      }

      PostCalledMethods[currentBranch] = lub; 

      for (CFG* succ : cfg->getSuccessors()) {
        logout("analyzing succesor " << succ->getBranchName())
        analyzeCFG(succ, PreCalledMethods, PostCalledMethods, AliasedVars, currentBranch); 
      }
      

    }
    else {
      logout("doing normal flow for " << currentBranch << " and prior = " << priorBranch)  

      std::map<std::string, CM> priorPostCM = std::map<std::string, CM>(PostCalledMethods[priorBranch]); 

      PreCalledMethods[currentBranch] = std::map<std::string, CM>(priorPostCM); 
      llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 

      std::map<std::string, CM> flowInto = std::map<std::string, CM>(PostCalledMethods[priorBranch]); 

       for (Instruction* I : instructions) {
        transfer(I, instructions, flowInto, AliasedVars);
      }

      PostCalledMethods[currentBranch] = flowInto;

      

      for (CFG* succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreCalledMethods, PostCalledMethods, AliasedVars, currentBranch); 
      }

    }

  }

}


void CalledMethodsAnalysis::doAnalysis(Function &F, PointerAnalysis *PA, std::string optLoadFileName) {
  SetVector<Instruction *> WorkSet;
  SetVector<Value *> PointerSet;
  std::string fnName = F.getName().str(); 

  int slashCount = 0; 
  std::string testName = getTestName(optLoadFileName); 

  bool functionIsKnown = false; 
  logout("fnname = " << fnName << " opt load file name = " << testName)

  loadFunctions();
  loadExpectedResult(testName, ExpectedResult);
 

  if (!ALLOW_REDEFINE) {
        // check if code re-defines a safe function or memory function. if so, cast it as a unsafe function
    if (SafeFunctions[fnName]) {
      SafeFunctions[fnName] = false; 
      UnsafeFunctions[fnName] = true; 
      functionIsKnown = true; 
      errs() << "CME-WARNING: Re-definition of safe function '" << fnName << "' identified on  and will be labelled as unsafe.\n";
    }

    for (auto Pair : MemoryFunctions) {
      if (fnName == Pair.first) {
        UnsafeFunctions[fnName] = true; 
        functionIsKnown = true; 
        errs() << "**CME-WARNING**: Re-definition of allocation function '" << fnName << "' identified and will be labelled as unsafe.\n";
      }

      if (fnName == Pair.second) {
        UnsafeFunctions[fnName] = true; 
        functionIsKnown = true; 
        errs() << "**CME-WARNING**: Re-definition of deallocation function '" << fnName << "' identified and will be labelled as unsafe.\n";
      }
    }
  }

  if (fnName == "main") {
    functionIsKnown = true; 
  }

  if (!functionIsKnown && !ALLOW_REDEFINE) {
    errs() << "CME-WARNING: Unknown function '" << fnName << "' identified and will be labelled as unsafe.\n";
    UnsafeFunctions[fnName] = true; 
  }

  

  if (fnName != "main") return; 

  AliasMap AliasedVars; 
  std::map<std::string, InstructionHolder> branchInstructionMap; 
  
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    WorkSet.insert(&(*I));
    PointerSet.insert(&(*I));

    std::string branchName = I->getParent()->getName().str();
    populateAliasedVars(&(*I), WorkSet, AliasedVars); 

    auto succs = getSuccessors(&(*I)); 
    branchInstructionMap[branchName].branch.insert(&(*I)); 
    for (auto succ : succs) {
      branchInstructionMap[branchName].successors.insert(succ); 
    }

  }



  CFG TopCFG; 
  buildCFG(TopCFG, realBranchOrder, branchInstructionMap); 

  CalledMethods PreCalledMethods; 
  CalledMethods PostCalledMethods; 

  // analyzeCFG(&TopCFG, PreCalledMethods, PostCalledMethods, AliasedVars, ""); 


  logout("\n\nPOST CALLED METHODS")
  for (auto Pair1 : PostCalledMethods) {
    std::string branchName = Pair1.first; 
    logout("branch = " << branchName)
    for (auto Pair2 : Pair1.second) {
      std::string cm; 
      for (auto m : Pair2.second.cmSet) {
        cm += m + ", "; 
      }
      logout(">> var name = " << Pair2.first << " cm = " << cm)
    }
  }

  logout("aliased vars")
  for (auto Pair : AliasedVars) {
    logout(Pair.first << " -> " << Pair.second)
  }

  errs() << "\n\nRUNNING TESTS - ALLOWED_REDEFINE = " << ALLOW_REDEFINE << " TEST NAME - " << testName << "\n\n";
  std::exit(runTests(ExpectedResult, PostCalledMethods));

 



}

} // namespace dataflow