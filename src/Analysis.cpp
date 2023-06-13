#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "loadFunctions.h"
#include "CFG.h"

#include <map> 
#include <tuple>
#include <vector> 
#include <list>

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

struct CM {
  std::set<std::string> cmSet; 
  bool setInitialized; 
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

    //   store i8* %call, i8** %str, align 8, !dbg !17

    Value* valueToStore = Store->getOperand(0);       // i8* %call
    Value* receivingValue = Store->getOperand(1);   // i8** %str
    
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

        }

      }
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

        // * (i think this handles) cases where an entirely random and undefined (implicitly declared) function shows up 
        if (Call->getCalledFunction() == NULL) {
          const DebugLoc &debugLoc = I->getDebugLoc();
          std::string location = "Line " + std::to_string(debugLoc.getLine()) + ", Col " + std::to_string(debugLoc.getCol());

          errs() << "CME-WARNING: unknown (unforseen occurence) & unsafe func on " << location << ". must call set for '" << argName << "' set to empty.\n";
          calledMethodsSet[argName].cmSet = {}; 
          
          return; 
        }

        std::string fnName = Call->getCalledFunction()->getName().str();


        if (UnsafeFunctions[fnName]) {
          logout("unsafe func, cm set emptied")
          calledMethodsSet[argName].cmSet = {}; 

          return; 
        }

        if (ReallocFunctions[fnName]) {
          logout("realloc func, cm set emptied")
          calledMethodsSet[argName].cmSet = {};

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
            calledMethodsSet[argName].cmSet.insert(fnName); 
            calledMethodsSet[argName].setInitialized = true; 
            break; 

          }
        }
        



    }
  }
  else if (auto Branch = dyn_cast<BranchInst>(I)) {

  }


}

void analyzeCFG(CFG* cfg, CalledMethods& PreCalledMethods, CalledMethods& PostCalledMethods, std::list<std::string> branchesAnalyzed, AliasMap& AliasedVars, std::string priorBranch) {
  std::string currentBranch =  cfg->getBranchName(); 

  std::string m; 
  for (auto k : branchesAnalyzed) {
    m += k + ", ";
  }
  logout("branches analyzed = " << m)

  if (currentBranch == "entry") { 

    PreCalledMethods[currentBranch] = {}; 
    llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 

    for (Instruction* I : instructions) {
      transfer(I, instructions, PostCalledMethods[currentBranch], AliasedVars);
    }

    branchesAnalyzed.push_back(currentBranch); 

    for (CFG* succ : cfg->getSuccessors()) {
      analyzeCFG(succ, PreCalledMethods, PostCalledMethods, branchesAnalyzed, AliasedVars, currentBranch); 
    }

  }

  else {
    std::map<std::string, CM> PriorPreCM = std::map<std::string, CM>(PreCalledMethods[currentBranch]); 
    std::map<std::string, CM> PriorPostCM = std::map<std::string, CM>(PostCalledMethods[currentBranch]); 

    bool foundCurrent = std::find(branchesAnalyzed.begin(), branchesAnalyzed.end(), currentBranch) != branchesAnalyzed.end();

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

      branchesAnalyzed.push_back(currentBranch); 

      for (CFG* succ : cfg->getSuccessors()) {
        bool found = std::find(branchesAnalyzed.begin(), branchesAnalyzed.end(), succ->getBranchName()) != branchesAnalyzed.end();
        if (!found) {
          logout("analyzing succesor " << succ->getBranchName())
          analyzeCFG(succ, PreCalledMethods, PostCalledMethods, branchesAnalyzed, AliasedVars, currentBranch); 
        }
        else {
          logout("not analyzing successor " << succ->getBranchName())
        }
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

      branchesAnalyzed.push_back(currentBranch); 
      

      for (CFG* succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreCalledMethods, PostCalledMethods, branchesAnalyzed, AliasedVars, currentBranch); 
      }

    }

  }

}


void CalledMethodsAnalysis::doAnalysis(Function &F, PointerAnalysis *PA) {
  SetVector<Instruction *> WorkSet;
  SetVector<Value *> PointerSet;

  
  std::string fnName = F.getName().str(); 
  bool functionIsKnown = false; 
  logout("fnname = " << fnName)

  loadFunctions();

  // check if code re-defines a safe function or memory function. if so, cast it as a unsafe function
  if (SafeFunctions[fnName]) {
    SafeFunctions[fnName] = false; 
    UnsafeFunctions[fnName] = true; 
    functionIsKnown = true; 
    errs() << "CME-WARNING: Re-definition of safe function '" << fnName << "' identified on  and will be labelled as unsafe.\n";
  }

  bool ALLOW_REDEFINE = true;  // ONLY for debugging purposes 

  if (!ALLOW_REDEFINE) {
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

  struct InstructionHolder {
    SetVector<Instruction*> branch; 
    SetVector<Instruction*> successors; 
  }; 

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

  


  CFG TopCFG = CFG("entry"); 

  for (auto branchName : realBranchOrder) {  
    auto succs = branchInstructionMap[branchName].successors; 



   CFG* cfg = TopCFG.getFind(branchName);   

   cfg->setInstructions(branchInstructionMap[branchName].branch);


   for (auto succ : succs) {
    std::string succName = succ->getParent()->getName().str();


    if (succName == branchName) continue; 


    if (cfg->checkFind(succName)) {
      cfg->addSuccessor(cfg->getFind(succName));
      cfg->getFind(succName)->addPredecessor(cfg);
      continue; 
    } 

    cfg->addSuccessor(succName); 
    cfg->getFind(succName)->addPredecessor(cfg);
  

   }

  }

  CalledMethods PreCalledMethods; 
  CalledMethods PostCalledMethods; 
  std::list<std::string> branchesAnalyzed; 

  analyzeCFG(&TopCFG, PreCalledMethods, PostCalledMethods, branchesAnalyzed, AliasedVars, ""); 


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

  logout("\n\nLAST BRANCH CALLED METHODS = " << realBranchOrder.back())
  for (auto Pair : PostCalledMethods[realBranchOrder.back()]) {

      std::string cm; 
      for (auto m : Pair.second.cmSet) {
        cm += m + ", "; 
      }
      logout(">> var name = " << Pair.first << " cm = " << cm)
    

  }



}

} // namespace dataflow