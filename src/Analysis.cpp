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

typedef std::map<std::string, dataflow::CalledMethodsEstimate> CME; 
typedef std::map<std::string, std::map<std::string, dataflow::CalledMethodsEstimate>> CallMethodsMap; // left = branch name. inner left = var name 
typedef std::map<std::string, std::string> AliasMap;
typedef std::map<std::string, std::string> VarBranchMap;

// TODO: handle branching (how do we handling branching?)
// TODO: handle switch statements 
// TODO: how final result is stored will need to change. i want to make it so that for every memory or unsafe function call, dataflow facts are added, INCLUDING information about branching. at the end of the code block, we then conclude if must calls are satisfied. 
// TODO: do we handle cases where it might call a dealloction function twice? 
// TODO: handle while loops 
// TODO!: currently "realloc" is labelled unsafe in unsafe.txt. for the sake of having results, it'll remain this way, but this is to change soon to be under realloc.txt instead. "unsafe.txt" end up not being useful. unsafe.txt may be renamed to "unknown" or "unpredictable", to better represent that these are a collection of functions we know may or may not change the must call satisfy of an expression  

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
  std::ifstream memoryFunctionsFile("../src/Functions/memory.txt");

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


void transfer(Instruction* I, SetVector<Instruction *>& workSet, CallMethodsMap& calledMethodsEst, AliasMap& aliasedVars, VarBranchMap& varsBranchMap) {
  std::string branchName = I->getParent()->getName().str();
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
  if (preds.size() == 1) { // if.then or if.else 
    // load predecessor into new branch 
    auto pred = preds[0];
    std::string predBranchName = pred->getParent()->getName().str(); 
    auto predCMEMap = calledMethodsEst[predBranchName]; 

    for (auto Pair : predCMEMap) {
      std::string predVarName = Pair.first; 
      dataflow::CalledMethodsEstimate predCME = Pair.second; 

      calledMethodsEst[branchName][predVarName] = {
        std::string(predCME.allocationFunction), 
        std::string(predCME.deallocationFunction), 
        Domain(predCME.calledMethodsState.Value)
      };

    }

  }
  else if (preds.size() == 2) { // if.end
    // lub predecesorrs into new branch 
    auto pred1 = preds[0];
    auto pred2 = preds[1]; 

    std::string pred1BranchName = pred1->getParent()->getName().str(); 
    std::string pred2BranchName = pred2->getParent()->getName().str(); 

    auto pred1CMEMap = calledMethodsEst[pred1BranchName]; 
    auto pred2CMEMap = calledMethodsEst[pred2BranchName]; 

    for (auto Pair1 : pred1CMEMap) {
      std::string pred1VarName = Pair1.first; 
      dataflow::CalledMethodsEstimate pred1CME = Pair1.second; 
      
      if (calledMethodsEst[pred2BranchName][pred1VarName].allocationFunction.size() > 0) {

        calledMethodsEst[branchName][pred1VarName] = {
          std::string(pred1CME.allocationFunction), 
          std::string(pred1CME.deallocationFunction), 
          Domain::join(pred1CME.calledMethodsState.Value, calledMethodsEst[pred2BranchName][pred1VarName].calledMethodsState.Value)
        };
      }
    
    }

    for (auto Pair2 : pred2CMEMap) {
        std::string pred2VarName = Pair2.first; 
        dataflow::CalledMethodsEstimate pred2CME = Pair2.second; 

        if (calledMethodsEst[branchName][pred2VarName].allocationFunction.size() == 0 &&
            pred2CME.allocationFunction.size() > 0 
        
        ) {

          calledMethodsEst[branchName][pred2VarName] = {
            std::string(pred2CME.allocationFunction), 
            std::string(pred2CME.deallocationFunction), 
            Domain(pred2CME.calledMethodsState.Value)
          };
        }


    }

  }


   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      logout("allocate inst, name = " << ("%" + Alloca->getName()))
      calledMethodsEst[branchName]["%" + Alloca->getName().str()] = {};
      calledMethodsEst[branchName]["%" + Alloca->getName().str()].calledMethodsState = Domain(Domain::Element::Top);

      varsBranchMap["%" + Alloca->getName().str()] = branchName;

      
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
               calledMethodsEst[branchName][varName].allocationFunction = fnName;
               calledMethodsEst[branchName][varName].deallocationFunction = MemoryFunctions[fnName];

               varsBranchMap[varName] = branchName; 

               // unsure if this should be in or out of if branch
               calledMethodsEst[branchName][varName].calledMethodsState.Value = Domain::Element::NeedsCall;

               logout("fn name true for " << branchName << " " << varName)
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
            
            if (calledMethodsEst[branchName][argName].allocationFunction.size() > 0) {
              calledMethodsEst[branchName][argName].calledMethodsState.selfApplyUnsafe(); 
            }
            return; 
          }

          std::string fnName = Call->getCalledFunction()->getName().str();

          logout("fnName for call = " << fnName)

          if (UnsafeFunctions[fnName]) {
            logout("unsafe func")
            if (calledMethodsEst[branchName][argName].allocationFunction.size() > 0) {
              calledMethodsEst[branchName][argName].calledMethodsState.selfApplyUnsafe(); 
            }
            return; 
          }

          if (ReallocFunctions[fnName]) {
            logout("realloc func")
            if (calledMethodsEst[branchName][argName].allocationFunction.size() > 0) {
              calledMethodsEst[branchName][argName].calledMethodsState.selfApplyReallocate(); 
            }
            return; 
          }

          if (SafeFunctions[fnName]) {
            logout("safe func")
            return; 
          }


          for (auto Pair : MemoryFunctions) {
            logout("pairs and fn name = " << Pair.first << " " << Pair.second << " " << fnName)
            if (fnName == Pair.first) { // allocation function
              calledMethodsEst[branchName][argName].allocationFunction = fnName; 
              calledMethodsEst[branchName][argName].deallocationFunction = MemoryFunctions[fnName];
              calledMethodsEst[branchName][argName].calledMethodsState.Value = Domain::Element::NeedsCall;

              varsBranchMap[argName] = branchName; // * questionable utility 
            }
            else if (fnName == Pair.second) { // deallocation function 
               calledMethodsEst[branchName][argName].calledMethodsState.selfApplyDeallocate();
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

  if (fnName == "main") {
    functionIsKnown = true; 
  }

  if (!functionIsKnown) {
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

  CallMethodsMap calledMethodsEstimate; 
  AliasMap AliasedVars; 
  VarBranchMap vbm; 

  for (auto I : WorkSet) {
   
    transfer(I, WorkSet, calledMethodsEstimate, AliasedVars, vbm);
  }


  bool FILTER_OUTPUT = false; 

  errs() << "\n\n CME \n\n";  
  for (auto branch : realBranchOrder) {
    errs() << "branch name = " << branch << "\n";
    for (auto Pair1 : calledMethodsEstimate[branch]) {

      if (FILTER_OUTPUT && Pair1.second.allocationFunction.size() == 0) continue; 

      errs() << "> var name = " << Pair1.first << "\n";
      errs() << ">> cme = " << Pair1.second.allocationFunction << " " << Pair1.second.deallocationFunction << " " << Pair1.second.calledMethodsState << "\n\n";
    }
    errs() << "\n";
  }



 

  


}

} // namespace dataflow