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
typedef std::map<std::string, std::string> AliasMap;
typedef std::map<std::string, std::string> VarBranchMap;

// TODO: handle branching (how do we handling branching?)
// TODO: handle switch statements 
// TODO: how final result is stored will need to change. i want to make it so that for every memory or unsafe function call, dataflow facts are added, INCLUDING information about branching. at the end of the code block, we then conclude if must calls are satisfied. 
// TODO: do we handle cases where it might call a dealloction function twice? 
// TODO: handle while loops 

namespace dataflow {

std::map<std::string, bool> SafeFunctions;
std::map<std::string, bool> UnsafeFunctions;
std::map<std::string, std::string> MemoryFunctions;

void loadFunctions() { 
  // working directory is /build 

  std::ifstream safeFunctionsFile("../src/Functions/safe.txt");
  std::ifstream unsafeFunctionsFile("../src/Functions/unsafe.txt");
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


void transfer(Instruction* I, SetVector<Instruction *>& workSet, CME& calledMethodsEst, AliasMap& aliasedVars, VarBranchMap& varsBranchMap) {
  std::string branchName = I->getParent()->getName().str();

   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      logout("allocate inst, name = " << ("%" + Alloca->getName()))
      calledMethodsEst["%" + Alloca->getName().str()] = {};
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


            if (
              !UnsafeFunctions[fnName] && 
              !UnsafeFunctions[MemoryFunctions[fnName]] && 
              MemoryFunctions.count(fnName) != 0
            ) {
               calledMethodsEst[varName].allocationFunction = fnName;
               calledMethodsEst[varName].deallocationFunction = MemoryFunctions[fnName];

               varsBranchMap[varName] = branchName; 
            }

            calledMethodsEst[varName].estimates[branchName] = false; 


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
            
            calledMethodsEst[argName].estimates[branchName] = false; 
            return; 
          }

          std::string fnName = Call->getCalledFunction()->getName().str();

          logout("fnName for call = " << fnName)

          if (UnsafeFunctions[fnName]) {
            logout("unsafe func")
            calledMethodsEst[argName].estimates[branchName] = false; 
            return; 
          }

          if (SafeFunctions[fnName]) {
            logout("safe func")
            return; 
          }


          for (auto Pair : MemoryFunctions) {
            logout("pairs and fn name = " << Pair.first << " " << Pair.second << " " << fnName)
            if (fnName == Pair.first) { // allocation function
              calledMethodsEst[argName].allocationFunction = fnName;
              calledMethodsEst[argName].deallocationFunction = MemoryFunctions[fnName];
              calledMethodsEst[argName].estimates[branchName] = false; 

              varsBranchMap[argName] = branchName; // * questionable utility 
            }
            else if (fnName == Pair.second) { // deallocation function 
              calledMethodsEst[argName].estimates[branchName] = true; 
            }
          }
          



      }
    }
    else if (auto Branch = dyn_cast<BranchInst>(I)) {

    }

    // check to see if we didn't before handle a branch 
    for (auto Pair : calledMethodsEst) {
      std::string varName = Pair.first; 
      std::string branchOfVar = varsBranchMap[varName]; 
      CalledMethodsEstimate cme = Pair.second; 
      std::vector<Instruction *> preds = getPredecessors(I);
      bool moveForward = false; 

      for (auto pred : preds) {
        std::string predName = pred->getParent()->getName().str(); 
        if (predName == branchOfVar) {
          moveForward = true; 
          break; 
        }
      }


      if (cme.estimates.count(branchName) == 0 && moveForward && cme.allocationFunction.size() != 0 && cme.deallocationFunction.size() != 0) {
        logout("currently missing branch (may be defined later but for now will be set to false) " << varName << " " << branchName)
        calledMethodsEst[varName].estimates[branchName] = false; 
      }

    }

}


void MustCallAnalysis::doAnalysis(Function &F, PointerAnalysis *PA) {
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

  CME calledMethodsEstimate; 
  AliasMap AliasedVars; 
  VarBranchMap vbm; 

  for (auto I : WorkSet) {
    transfer(I, WorkSet, calledMethodsEstimate, AliasedVars, vbm);
  }


  // reasoning about if-statements 
  for (auto Pair : calledMethodsEstimate) {
    std::string varName = Pair.first; 
    std::string branchOfVarName = vbm[varName];
    CalledMethodsEstimate cme = Pair.second; 
    bool isFinalMustCallSatisfied = true; 
    bool branchCallsUnsafe = false;
    bool allIfEndsSatisfied = NULL; 


    for (auto Pair2 : cme.estimates) {
      std::string branchName = Pair2.first; 
      bool mustCallSatisfied = Pair2.second; 

      if (branchName.substr(0, 6).compare("if.end") == 0) {
        if (mustCallSatisfied && allIfEndsSatisfied == NULL) {
          allIfEndsSatisfied = true; 
        }
        else if (!mustCallSatisfied && allIfEndsSatisfied == true) {
          allIfEndsSatisfied = false; 
        }

      }

      if (branchName == branchOfVarName && mustCallSatisfied && cme.estimates.size() == 1) {
          isFinalMustCallSatisfied = true; 
          break;
      }


      if (!mustCallSatisfied && branchName != branchOfVarName) {
        isFinalMustCallSatisfied = false; 
      }

    }

    if (allIfEndsSatisfied == true) {
      isFinalMustCallSatisfied = true; 
    }

    calledMethodsEstimate[varName].finalMustCallIsSatisfied = isFinalMustCallSatisfied;

  }

   logout("\n\n#### CME #####\n\n")
   for (auto Pair : calledMethodsEstimate) {
    if (Pair.first.size() == 0 || Pair.second.allocationFunction.size() == 0 || Pair.second.deallocationFunction.size() == 0) continue; 
    
      errs() << Pair.first << " = " << Pair.second.allocationFunction << " " << Pair.second.deallocationFunction << ":\n";
      for (auto Pair2 : Pair.second.estimates) {
        errs() << Pair2.first << " must call sat = " << Pair2.second << "\n";
      }
      errs() << "Concludes: " << Pair.second.finalMustCallIsSatisfied << "\n\n";
   }

   // if it concludes 1, then respective string has its must call satisfied 


 

  


}

} // namespace dataflow