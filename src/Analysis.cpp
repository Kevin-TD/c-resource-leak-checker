#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "loadFunctions.h"

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

// TODO: handle switch statements 
// TODO: handle while loops 
// should we have to worry if a deallocation function is called within the for loop declaration? like a for ( ... ; ... ; free(...)) ? same for while loops

// worth noting that has an expr is declared when IR does "call void @llvm.dbg.declare [other IR ...]"

// assumptions: 
// if.end statements have 2 preds
// if.then, if.else statements have 1 pred

namespace dataflow {

std::map<std::string, bool> SafeFunctions;
std::map<std::string, bool> UnsafeFunctions;
std::map<std::string, bool> ReallocFunctions;
std::map<std::string, std::string> MemoryFunctions;
std::vector<std::string> realBranchOrder; // ONLY for debugging purposes 

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

void loadForLoopBodyBranchNames(Instruction* Inst, std::set<std::string>& branches) {

  auto preds = getPredecessors(Inst); 

  for (auto pred : preds) {
    std::string predName = pred->getParent()->getName().str(); 
    
    if (predName.substr(0, 7).compare("for.inc") != 0  && predName.substr(0, 8).compare("for.cond") != 0) {
      branches.insert(predName);
    }


    if (predName.substr(0, 8).compare("for.body") == 0) {
      return; 
    }

    loadForLoopBodyBranchNames(pred, branches); 

  }
  

}


void transfer(Instruction* I, SetVector<Instruction *>& workSet, CalledMethods& calledMethodsEst, AliasMap& aliasedVars) {
  std::string branchName = I->getParent()->getName().str();
  logout("inst " << *I << " branch name = " << branchName)
  bool includes = false; 
  for (auto branch : realBranchOrder) {
    if (branch == branchName) {
      includes = true; 
      break;
    }
  }
  if (!includes) {
    realBranchOrder.push_back(branchName);
  }

  auto preds = getPredecessors(I);

  // for a for.end, it should be lub of

  if (preds.size() == 1 && 
    (branchName.substr(0, 7).compare("if.then") == 0 ||  branchName.substr(0, 7).compare("if.else") == 0)) { 
    // load predecessor into new branch 
    auto pred = preds[0];
    std::string predBranchName = pred->getParent()->getName().str(); 
    auto predCM = calledMethodsEst[predBranchName]; 

    for (auto Pair : predCM) {
      std::string predVarName = Pair.first; 
      CM predCalledMethods = Pair.second; 

      calledMethodsEst[branchName][predVarName] = {
        std::set<std::string>(predCalledMethods.cmSet), 
        true 
      };

    }

  }
  else if (preds.size() == 2 && branchName.substr(0, 6).compare("if.end") == 0) { 
    // lub predecesorrs into new branch 
    auto pred1 = preds[0];
    auto pred2 = preds[1]; 

    std::string pred1BranchName = pred1->getParent()->getName().str(); 
    std::string pred2BranchName = pred2->getParent()->getName().str(); 

    auto pred1CMMap = calledMethodsEst[pred1BranchName]; 
    auto pred2CMMap = calledMethodsEst[pred2BranchName]; 
    std::map<std::string, std::map<std::string, bool>> assignedVars;

    for (auto Pair1 : pred1CMMap) {
      std::string pred1VarName = Pair1.first; 
      CM pred1CM = Pair1.second; 
      
        
      // intersection 
      std::set<std::string> intersection;
      std::set_intersection(pred1CM.cmSet.begin(), pred1CM.cmSet.end(), 
                    calledMethodsEst[pred2BranchName][pred1VarName].cmSet.begin(), calledMethodsEst[pred2BranchName][pred1VarName].cmSet.end(), 
                    std::inserter(intersection, intersection.begin()));


      calledMethodsEst[branchName][pred1VarName] = {
        intersection, 
        true
      }; 
      
    
    }

    // fill in the rest
    for (auto Pair2 : pred2CMMap) {
        std::string pred2VarName = Pair2.first; 
        CM pred2CM = Pair2.second; 

        if (!calledMethodsEst[branchName][pred2VarName].setInitialized) {

          calledMethodsEst[branchName][pred2VarName] = {
            std::set<std::string>(pred2CM.cmSet), 
            true 
          };
          
        }


    }

  }
  else if (branchName.substr(0, 7).compare("for.end") == 0) {
    std::set<std::string> branches; 
    loadForLoopBodyBranchNames(I, branches); 
    
    std::map<std::string, std::map<std::string, bool>> assignedVars;

    for (std::string predBranch : branches) {

      logout("pred branch = " << predBranch << " main " << branchName)

      auto cmMap = calledMethodsEst[predBranch];
      for (auto pair : cmMap) {
        std::string varName = pair.first; 
        CM cm = pair.second; 

        if (!calledMethodsEst[branchName][varName].setInitialized) {
          calledMethodsEst[branchName][varName] = {
            std::set<std::string>(pair.second.cmSet), 
            true 
          };

        }
        else {

          if (calledMethodsEst[predBranch][varName].setInitialized) {
            std::set<std::string> intersection;
            std::set_intersection(calledMethodsEst[branchName][varName].cmSet.begin(), calledMethodsEst[branchName][varName].cmSet.end(), 
                      calledMethodsEst[predBranch][varName].cmSet.begin(), calledMethodsEst[predBranch][varName].cmSet.end(), 
                      std::inserter(intersection, intersection.begin()));

            calledMethodsEst[branchName][varName].cmSet = intersection;
          }

        }

      }
    }
   
  }


   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      logout("allocate inst, name = " << ("%" + Alloca->getName()))
      calledMethodsEst[branchName]["%" + Alloca->getName().str()] = {};
      
    }
    else if (auto Load = dyn_cast<LoadInst>(I)) {
      logout("(load) name is " << variable(Load) << " for " << variable(Load->getPointerOperand()) )
      aliasedVars[variable(Load)] = variable(Load->getPointerOperand()); 
    }
    else if (auto Store = dyn_cast<StoreInst>(I)) {

      //   store i8* %call, i8** %str, align 8, !dbg !17

      Value* valueToStore = Store->getOperand(0);       // i8* %call
      Value* receivingValue = Store->getOperand(1);   // i8** %str

      logout("value to store = " << valueToStore << " !!!! " << *valueToStore)
      logout("value that's receiving = " << variable(receivingValue))

      
      for (auto Inst : workSet) {
        if (valueToStore == Inst) {
          if (auto Call = dyn_cast<CallInst>(Inst)) {
            std::string varName = variable(Store->getOperand(1)); 
            std::string fnName = Call->getCalledFunction()->getName().str(); 
            logout("fn name = " << fnName)

            if (
              UnsafeFunctions.count(fnName) == 0  && 
              UnsafeFunctions.count(MemoryFunctions[fnName]) == 0 && 
              MemoryFunctions[fnName].size() > 0
            ) {
              //  calledMethodsEst[branchName][varName].insert(fnName);   
               logout("fn name true for " << branchName << " " << varName)
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

          if (aliasedVars.count(argName) != 0) {
            argName = aliasedVars[argName];
          }

          logout("arg = " << argName) 

          // * i THINK this is what distinguishes "real" variables defined in the program from constants and other irrelevant llvm stuff 
          if (argName[0] != '%') return; 

          // * (i think this handles) cases where an entirely random and undefined (implicitly declared) function shows up 
          if (Call->getCalledFunction() == NULL) {
            const DebugLoc &debugLoc = I->getDebugLoc();
            std::string location = "Line " + std::to_string(debugLoc.getLine()) + ", Col " + std::to_string(debugLoc.getCol());

            errs() << "CME-WARNING: unknown (unforseen occurence) & unsafe func on " << location << ". must call satisfied property for '" << argName << "' set to false.\n";
            
            return; 
          }

          std::string fnName = Call->getCalledFunction()->getName().str();

          logout("fnName for call = " << fnName << " " << branchName)

          if (UnsafeFunctions[fnName]) {
            logout("unsafe func")
            return; 
          }

          if (ReallocFunctions[fnName]) {
            logout("realloc func")
            // calledMethodsEst[branchName][argName].insert(fnName); 
            return; 
          }

          if (SafeFunctions[fnName]) {
            logout("safe func")
            return; 
          }


          for (auto Pair : MemoryFunctions) {
            logout("pairs and fn name = " << Pair.first << " " << Pair.second << " " << branchName << " " << argName)
            if (fnName == Pair.first) { // allocation function

            }
            else if (fnName == Pair.second) { // deallocation function 
              calledMethodsEst[branchName][argName].cmSet.insert(fnName); 
              calledMethodsEst[branchName][argName].setInitialized = true; 
              break; 

            }
          }
          



      }
    }
    else if (auto Branch = dyn_cast<BranchInst>(I)) {

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
  
  
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    WorkSet.insert(&(*I));
    PointerSet.insert(&(*I));
    logout("inst = " << *I);
    logout("block = " << I->getParent()->getName())
    logout("preds = ")
    std::vector<Instruction *> preds = getPredecessors(&(*I));
    for (auto pred : preds) {
      logout(pred->getParent()->getName())
    }
    logout("end of preds")

  }

  CalledMethods calledMethodsEstimate; 
  AliasMap AliasedVars; 

  for (auto I : WorkSet) {
   
    transfer(I, WorkSet, calledMethodsEstimate, AliasedVars);

  }

  errs() << "\n\n CME \n\n";  
  for (auto branch : realBranchOrder) {
    errs() << "branch name = " << branch << "\n";
    for (auto Pair1 : calledMethodsEstimate[branch]) {


      errs() << "> var name = " << Pair1.first << "\n";
      errs() << ">> cm = ";
      std::string cm; 
      for (auto s : Pair1.second.cmSet) {
        cm += s + ", ";
      }
      errs() << cm << "\n"; 
    }
    errs() << "\n";
  }

}

} // namespace dataflow