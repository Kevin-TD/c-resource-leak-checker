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

// sh ../run_test.sh 1

typedef std::map<std::string, dataflow::CalledMethodsEstimate> CME; 
typedef std::map<std::string, dataflow::rCalledMethodsEstimate> rCME; // TODO: rename and all that  
typedef std::map<std::string, std::string> AliasMap;
typedef std::map<std::string, std::string> VarBranchMap;

// TODO: handle branching (how do we handling branching?)
// TODO: handle switch statements 
// TODO: how final result is stored will need to change. i want to make it so that for every memory or unsafe function call, dataflow facts are added, INCLUDING information about branching. at the end of the code block, we then conclude if must calls are satisfied. 

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


void transfer(Instruction* I, CME& calledMethodsEst, AliasMap& aliasedVars, SetVector<Instruction *>& workSet, rCME& rCalledMethodsEst) {
  std::string branchName = I->getParent()->getName().str();

   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      logout("allocate inst, name = " << ("%" + Alloca->getName()))
      calledMethodsEst["%" + Alloca->getName().str()] = {};
      rCalledMethodsEst["%" + Alloca->getName().str()] = {};
      
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

             if (aliasedVars.count(varName) != 0) {
              logout("receiving aliased as " << aliasedVars[varName])
              varName = aliasedVars[varName];
            }

            
            std::string fnName = Call->getCalledFunction()->getName().str(); 


            if (
              !UnsafeFunctions[fnName] && 
              !UnsafeFunctions[MemoryFunctions[fnName]] && 
              MemoryFunctions.count(fnName) != 0
            ) {
               calledMethodsEst[varName].allocationFunction = fnName;
               calledMethodsEst[varName].deallocationFunction = MemoryFunctions[fnName];

               rCalledMethodsEst[varName].allocationFunction = fnName;
               rCalledMethodsEst[varName].deallocationFunction = MemoryFunctions[fnName];
            }

            calledMethodsEst[varName].mustCallIsSatisfied = false;  
            rCalledMethodsEst[varName].estimates[branchName] = false; 


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

            errs() << "WARNING: unknown (unforseen occurence) & unsafe func on " << location << ". must call satisfied property for '" << argName << "' set to false.\n";
            
            calledMethodsEst[argName].mustCallIsSatisfied = false; 
            rCalledMethodsEst[argName].estimates[branchName] = false; 
            return; 
          }

          std::string fnName = Call->getCalledFunction()->getName().str();

          logout("fnName for call = " << fnName)

          if (UnsafeFunctions[fnName]) {
            logout("unsafe func")
            calledMethodsEst[argName].mustCallIsSatisfied = false; 
            rCalledMethodsEst[argName].estimates[branchName] = false; 
            return; 
          }

          if (SafeFunctions[fnName]) {
            logout("safe func")
            return; 
          }


          for (auto Pair : MemoryFunctions) {
            logout("pairs& fn name = " << Pair.first << " " << Pair.second << " " << fnName)
            if (fnName == Pair.first) { // allocation function
              calledMethodsEst[argName].allocationFunction = fnName;
              calledMethodsEst[argName].deallocationFunction = MemoryFunctions[fnName];
              calledMethodsEst[argName].mustCallIsSatisfied = false; 

              rCalledMethodsEst[argName].allocationFunction = fnName;
              rCalledMethodsEst[argName].deallocationFunction = MemoryFunctions[fnName];
              rCalledMethodsEst[argName].estimates[branchName] = false; 
            }
            else if (fnName == Pair.second) { // deallocation function 
              calledMethodsEst[argName].mustCallIsSatisfied = true;  

              rCalledMethodsEst[argName].estimates[branchName] = true; 
            }
          }
          



      }
    }
    else if (auto Branch = dyn_cast<BranchInst>(I)) {
      // Assumptions about LLVM IR: an if statement will start with a label "%if.then"


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
    errs() << "WARNING: Re-definition of safe function '" << fnName << "' identified on  and will be labelled as unsafe.\n";
  }

  for (auto Pair : MemoryFunctions) {
    if (fnName == Pair.first) {
      UnsafeFunctions[fnName] = true; 
      functionIsKnown = true; 
      errs() << "**WARNING**: Re-definition of allocation function '" << fnName << "' identified and will be labelled as unsafe.\n";
    }

    if (fnName == Pair.second) {
      UnsafeFunctions[fnName] = true; 
      functionIsKnown = true; 
      errs() << "**WARNING**: Re-definition of deallocation function '" << fnName << "' identified and will be labelled as unsafe.\n";
    }
  }

  if (fnName == "main") {
    functionIsKnown = true; 
  }

  if (!functionIsKnown) {
    errs() << "WARNING: Unknown function '" << fnName << "' identified and will be labelled as unsafe.\n";
    UnsafeFunctions[fnName] = true; 
  }

  

  if (fnName != "main") return; 
  
  
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    WorkSet.insert(&(*I));
    PointerSet.insert(&(*I));
    logout("inst = " << *I);
    logout("block =" << I->getParent()->getName())
  }

  CME calledMethodsEstimate; 
  rCME myRCalledMethodsEstimate;
  AliasMap AliasedVars; 
  VarBranchMap vbm; 

  for (auto I : WorkSet) {
    transfer(I, calledMethodsEstimate, AliasedVars, WorkSet, myRCalledMethodsEstimate);
  }

  logout("\n\n#### end of execution #####\n\n")

  for (auto Pair : calledMethodsEstimate) {
    errs() << Pair.first << " = " << Pair.second.allocationFunction << " " << Pair.second.deallocationFunction << " " << Pair.second.mustCallIsSatisfied << " \n";
  }

   logout("\n\n#### RCME #####\n\n")
   for (auto Pair : myRCalledMethodsEstimate) {
      errs() << Pair.first << " = " << Pair.second.allocationFunction << " " << Pair.second.deallocationFunction << ":\n";
      for (auto Pair2 : Pair.second.estimates) {
        errs() << Pair2.first << " " << Pair2.second << "\n";
      }
   }

   // if statement collapse logic: 
   // if CME concludes that in the branch where the variable is defined, the must calls are satisfied, then the must call is definitely satisfied 
   // if not, then for all other branches, if the must call is satisfied, then the must call is definitely satisfied. 


   logout("\n\n#### filtered output #####\n\n")

   for (auto Pair : calledMethodsEstimate) {
    if (Pair.first.size() == 0 || Pair.second.allocationFunction.size() == 0 || Pair.second.deallocationFunction.size() == 0) continue; 

    errs() << Pair.first << " = " << Pair.second.allocationFunction << " " << Pair.second.deallocationFunction << " " << Pair.second.mustCallIsSatisfied << " \n";
    
  }

}

} // namespace dataflow