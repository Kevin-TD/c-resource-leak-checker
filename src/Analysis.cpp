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



typedef std::map<std::string, dataflow::MustCallEstimate> MustCallEst; 
typedef std::map<std::string, std::string> AliasMap;

// TODO: handle branching (how do we handling branching?)

namespace dataflow {

std::vector<std::string> SafeFunctions = { "strcpy", "printf" }; 
std::vector<std::string> UnsafeFunctions = { "realloc" }; 
std::vector<std::string> AllocationFunctions = { "malloc" }; 
std::vector<std::string> DeallocationFunctions = { "free" }; 


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


void transfer(Instruction* I, MustCallEst& mustCallEstimates, AliasMap& aliasedVars, SetVector<Instruction *>& workSet) {
  std::string blockName = I->getParent()->getName().str();

   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
      logout("allocate inst, name = " << ("%" + Alloca->getName()))
      mustCallEstimates["%" + Alloca->getName().str()] = {};
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
            const DebugLoc &debugLoc = Inst->getDebugLoc();
            std::string functionName = "{STORE} branch = " + blockName 
            + " Line = " + std::to_string(debugLoc.getLine()) + " Col = " + std::to_string(debugLoc.getCol()) 
            + " name = " + Call->getCalledFunction()->getName().str();

            std::string varName = variable(Store->getOperand(1)); 

             if (aliasedVars.count(varName) != 0) {
              logout("receiving aliased as " << aliasedVars[varName])
              varName = aliasedVars[varName];
            }

            logout("(instore) function name = " << functionName)

            // (assuming) these functions take no args, 0 used as dummy val 
            std::string fnName = Call->getCalledFunction()->getName().str(); 
            mustCallEstimates[varName].allocationFunction = fnName;
            mustCallEstimates[varName].mustCallIsSatisfied = false; 

            for (int i = 0; i < AllocationFunctions.size(); i++) {
              if (AllocationFunctions.at(i) == fnName) {
                mustCallEstimates[varName].deallocationFunction = DeallocationFunctions.at(i);
              }
            }


          }

        }
      }
    
    }
    else if (auto Call = dyn_cast<CallInst>(I)) {
      const DebugLoc &debugLoc = I->getDebugLoc();
      logout("debug loc = " << debugLoc.getLine() << " " << debugLoc.getCol())
      

      std::string functionName = "branch = " + blockName 
        + " Line = " + std::to_string(debugLoc.getLine()) + " Col = " + std::to_string(debugLoc.getCol()) 
        + " name = " + Call->getCalledFunction()->getName().str();

      logout("function name = " << functionName)

      for (unsigned i = 0; i < Call->getNumArgOperands(); ++i) {
          Value *argument = Call->getArgOperand(i);
          std::string argName = variable(argument);

          if (aliasedVars.count(argName) != 0) {
            argName = aliasedVars[argName];
          }

          logout("arg = " << argName << "|") 

          std::string fnName = Call->getCalledFunction()->getName().str();

          for (int i = 0; i < SafeFunctions.size(); i++) {
            if (fnName == SafeFunctions.at(i)) return; 
          }

          for (int i = 0; i < UnsafeFunctions.size(); i++) {
            if (fnName == SafeFunctions.at(i)) {
              mustCallEstimates[argName].mustCallIsSatisfied = false; 
            }
          }

          for (int i = 0; i < AllocationFunctions.size(); i++) {
            if (fnName == AllocationFunctions.at(i)) {
              mustCallEstimates[argName].allocationFunction = fnName;
              mustCallEstimates[argName].mustCallIsSatisfied = false; 
            }
            else if (fnName == DeallocationFunctions.at(i)) {
              mustCallEstimates[argName].mustCallIsSatisfied = true;  
            }
          }



      }
    }
    else if (auto Branch = dyn_cast<BranchInst>(I)) {
      
    }
}


void MustCallAnalysis::doAnalysis(Function &F, PointerAnalysis *PA) {
  SetVector<Instruction *> WorkSet;
  SetVector<Value *> PointerSet;

  if (F.getName() != "main") return; 
  
  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    WorkSet.insert(&(*I));
    PointerSet.insert(&(*I));
    logout("inst = " << *I << " " << I->getParent()->getName());
  }

  MustCallEst MustCallEstimates; 
  AliasMap AliasedVars; 

  for (auto I : WorkSet) {
    transfer(I, MustCallEstimates, AliasedVars, WorkSet);
  }

  logout("\n\n#### end of execution #####\n\n")

  for (auto Pair : MustCallEstimates) {
    errs() << Pair.first << " = " << Pair.second.allocationFunction << " " << Pair.second.deallocationFunction << " " << Pair.second.mustCallIsSatisfied << " \n";
  }

}

} // namespace dataflow