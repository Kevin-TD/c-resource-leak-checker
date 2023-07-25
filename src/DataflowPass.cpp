#include "DataflowPass.h"
#include "Debug.h"
#include "TestRunner.h"
#include "Utils.h"
#include <fstream>

void DataflowPass::setFunctions(
    std::set<std::string> safeFunctions, std::set<std::string> reallocFunctions,
    std::map<std::string, std::string> memoryFunctions) {
  this->safeFunctions = safeFunctions;
  this->reallocFunctions = reallocFunctions;
  this->memoryFunctions = memoryFunctions;
}

void DataflowPass::setExpectedResult(MappedMethods expectedResult) {
  this->expectedResult = expectedResult;
}

MappedMethods DataflowPass::generatePassResults() {
  MappedMethods PreMappedMethods;
  MappedMethods PostMappedMethods;
  this->analyzeCFG(this->cfg, PreMappedMethods, PostMappedMethods, "");
  return PostMappedMethods;
}

void DataflowPass::setCFG(CFG *cfg) { this->cfg = cfg; }

void DataflowPass::setAliasedVars(AliasMap aliasedVars) {
  this->aliasedVars = aliasedVars;
}

void DataflowPass::transfer(
    Instruction *instruction, SetVector<Instruction *> workSet,
    std::map<std::string, MaybeUninitMethodsSet> &inputMethodsSet) {

  if (auto Store = dyn_cast<StoreInst>(instruction)) {
    Value *valueToStore = Store->getOperand(0);
    Value *receivingValue = Store->getOperand(1);

    logout("\nvalue to store = " << *valueToStore)
        logout("receiving value = " << *receivingValue)

            for (auto Inst : workSet) {
      if (valueToStore == Inst) {
        if (auto Call = dyn_cast<CallInst>(Inst)) {
          /*
            For some x = malloc(params ...), we are only considering tracking x
            (argName)
            TODO: rename "argName" to "assignedVarName"
          */
          std::string argName = dataflow::variable(Store->getOperand(1));

          if (argName[0] == '@') {
            argName[0] = '%';
          }

          while (argName.size() > 1 && dataflow::isNumber(argName.substr(1))) {
            argName = aliasedVars[argName];
          }

          logout("arg name store inst call = " << argName
                                               << " and inst = " << *Inst)

              std::string fnName = Call->getCalledFunction()->getName().str();

          ProgramVariable assignedVar = ProgramVariable(Store->getOperand(1));
          std::set<std::string> allAliases =
              this->programVariables.findVarAndNamedAliases(
                  assignedVar.getCleanedName());
          for (auto a : allAliases) {
            logout("x100 store alias = " << a)
          }

          if (this->memoryFunctions[fnName].size() > 0) {
            for (std::string alias : allAliases) {
              logout("calling on alloc function for argname "
                     << alias << " and fnname " << fnName) this
                  ->onAllocationFunctionCall(inputMethodsSet[alias], fnName);
            }
          }
          break;
        } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(Inst)) {
          std::string argName = dataflow::variable(Store->getOperand(1));
          logout("arg name store inst bitcast = " << argName)

              if (argName[0] == '@') {
            argName[0] = '%';
          }

          if (CallInst *call = dyn_cast<CallInst>(bitcast->getOperand(0))) {
            std::string fnName = call->getCalledFunction()->getName().str();

            logout("fnname store inst bitcast = " << fnName)

                ProgramVariable bitcastVar =
                    ProgramVariable(Store->getOperand(1));
            std::set<std::string> allAliases =
                this->programVariables.findVarAndNamedAliases(
                    bitcastVar.getCleanedName());
            for (auto a : allAliases) {
              logout("x101 bitcast alias = " << a)
            }

            if (this->memoryFunctions[fnName].size() > 0) {
              for (auto alias : allAliases) {
                this->onAllocationFunctionCall(inputMethodsSet[alias], fnName);
              }
            }
            break;
          }
        }
      }
    }

    /*
    LLVM identifiers either start with @ (global identifier) or % (local
    identifier) Docs: https://llvm.org/docs/LangRef.html#identifiers For
    (temporary decision) simplicity sake, @'s will be replaced with %, though in
    the future the variable naming system may entirely change to remove %'s and
    @'s

    */

  } else if (auto Call = dyn_cast<CallInst>(instruction)) {
    for (unsigned i = 0; i < Call->getNumArgOperands(); ++i) {
      Value *argument = Call->getArgOperand(i);
      std::string argName = dataflow::variable(argument);

      argName = aliasedVars[argName];
      logout("arg number = " << i) logout("pre arg name = " << argName)

          while (argName.size() > 1 && dataflow::isNumber(argName.substr(1))) {
        argName = aliasedVars[argName];
        logout("arg name now " << argName)
      }

      if (argName == "") {
        continue;
      }

      std::list<std::string> allAliases2;

      while (argName != "") {
        allAliases2.push_back(argName);
        logout("arg name pushed " << argName) argName = aliasedVars[argName];
      }

      logout("for call inst " << *Call) for (auto alias : allAliases2){
          logout("alias = " << alias)}

      ProgramVariable functionCallVar = ProgramVariable(Call->getArgOperand(i));
      logout("cleaned name = " << functionCallVar.getCleanedName())
          std::set<std::string>
              allAliases = this->programVariables.findVarAndNamedAliases(
                  functionCallVar.getCleanedName());
      for (auto a : allAliases) {
        logout("x102 functionCallVar alias = " << a)
      }

      // * (i think this handles) cases where an undefined to us function shows
      // up
      if (Call->getCalledFunction() == NULL) {
        const DebugLoc &debugLoc = instruction->getDebugLoc();
        std::string location = "Line " + std::to_string(debugLoc.getLine()) +
                               ", Col " + std::to_string(debugLoc.getCol());

        errs() << "WARNING: implicitly declared function call on " << location
               << "\n";

        for (std::string aliasArg : allAliases) {
          this->onUnknownFunctionCall(inputMethodsSet[aliasArg]);
        }

        continue;
      }

      std::string fnName = Call->getCalledFunction()->getName().str();

      logout("call fnname = " << fnName)

          if (this->reallocFunctions.count(fnName)) {
        for (std::string aliasArg : allAliases) {
          this->onReallocFunctionCall(inputMethodsSet[aliasArg], fnName);
        }
        continue;
      }

      if (this->safeFunctions.count(fnName)) {
        for (std::string aliasArg : allAliases) {
          this->onSafeFunctionCall(inputMethodsSet[aliasArg], fnName);
        }
        continue;
      }

      // call void @free(i8* %7) #2, !dbg !291   <- args matter
      // %call5 = call noalias i8* @aligned_alloc(i64 %4, i64 %5) #2, !dbg !287
      // <- args do not matter
      /*
        we are making the following assumption:
        for deallocation functions, the arguments matter since they will contain
        the pointers/var names to memory for allocation functions, the arguments
        do not matter since they are typically numbers. the variable that is
        receiving the memory is handled elswhere (in the handling of store
        insts)
      */

      bool loopBroken = false;
      for (auto Pair : this->memoryFunctions) {
        if (fnName == Pair.first) {
          for (std::string aliasArg : allAliases) {
            logout("not calling onalloc for "
                   << aliasArg << " of fnName " << fnName << " and inst "
                   << *instruction << " and just returning now") return;
          }
        } else if (fnName == Pair.second) {
          for (std::string aliasArg : allAliases) {
            this->onDeallocationFunctionCall(inputMethodsSet[aliasArg], fnName);
          }
          loopBroken = true;
          break;
        }
      }

      if (loopBroken) {
        continue;
      }

      // only cases here are calls to functions not already seen before

      logout("also unknown functionc call " << fnName)
    }
  }
}

void DataflowPass::analyzeCFG(CFG *cfg, MappedMethods &PreMappedMethods,
                              MappedMethods &PostMappedMethods,
                              std::string priorBranch) {
  std::string currentBranch = cfg->getBranchName();

  if (currentBranch == "entry") {
    PreMappedMethods[currentBranch] = {};
    llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

    for (Instruction *instruction : instructions) {
      transfer(instruction, instructions, PostMappedMethods[currentBranch]);
    }

    for (CFG *succ : cfg->getSuccessors()) {
      analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch);
    }

  }

  else {
    std::map<std::string, MaybeUninitMethodsSet> PriorPreMC =
        std::map<std::string, MaybeUninitMethodsSet>(
            PreMappedMethods[currentBranch]);
    std::map<std::string, MaybeUninitMethodsSet> PriorPostMC =
        std::map<std::string, MaybeUninitMethodsSet>(
            PostMappedMethods[currentBranch]);

    if (PriorPreMC.size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch)

          std::map<std::string, MaybeUninitMethodsSet>
              CurrentPreMC = std::map<std::string, MaybeUninitMethodsSet>(
                  PostMappedMethods[priorBranch]);

      // check if inputs (pre) differ
      bool currentAndPriorPreMCEqual = true;
      for (auto Pair : CurrentPreMC) {
        std::string varName = Pair.first;
        MaybeUninitMethodsSet currentPreMC = Pair.second;

        if ((currentPreMC.methodsSet != PriorPreMC[varName].methodsSet) ||
            (currentPreMC.setInitialized !=
             PriorPreMC[varName].setInitialized)) {
          currentAndPriorPreMCEqual = false;
          break;
        }
      }

      for (auto Pair : PriorPreMC) {
        std::string varName = Pair.first;
        MaybeUninitMethodsSet priorPreMC = Pair.second;

        if ((priorPreMC.methodsSet != CurrentPreMC[varName].methodsSet) ||
            (priorPreMC.setInitialized !=
             CurrentPreMC[varName].setInitialized)) {
          currentAndPriorPreMCEqual = false;
          break;
        }
      }

      if (currentAndPriorPreMCEqual)
        return;

      // lub PriorPreCM and CurrentPreCM
      std::map<std::string, MaybeUninitMethodsSet> lub;

      for (auto Pair1 : PriorPreMC) {
        std::string varName = Pair1.first;
        MaybeUninitMethodsSet priorPreMC = Pair1.second;

        std::set<std::string> setUnion;

        this->leastUpperBound(priorPreMC, CurrentPreMC[varName], setUnion);

        lub[varName] = {setUnion, true};
      }

      // fill in the rest
      for (auto Pair1 : PriorPostMC) {
        std::string varName = Pair1.first;
        MaybeUninitMethodsSet priorPostMC = Pair1.second;

        if (!lub[varName].setInitialized) {
          lub[varName] = {std::set<std::string>(priorPostMC.methodsSet), true};
        }
      }

      PreMappedMethods[currentBranch] =
          std::map<std::string, MaybeUninitMethodsSet>(lub);

      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();
      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, lub);
      }

      PostMappedMethods[currentBranch] = lub;

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch);
      }

    } else {
      logout("doing normal flow for " << currentBranch
                                      << " and prior = " << priorBranch)

          std::map<std::string, MaybeUninitMethodsSet>
              priorPostCM = std::map<std::string, MaybeUninitMethodsSet>(
                  PostMappedMethods[priorBranch]);

      PreMappedMethods[currentBranch] =
          std::map<std::string, MaybeUninitMethodsSet>(priorPostCM);
      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

      std::map<std::string, MaybeUninitMethodsSet> flowInto =
          std::map<std::string, MaybeUninitMethodsSet>(
              PostMappedMethods[priorBranch]);

      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, flowInto);
      }

      PostMappedMethods[currentBranch] = flowInto;

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch);
      }
    }
  }
}

void DataflowPass::setProgramVariables(
    ProgramVariablesHandler programVariables) {
  this->programVariables = programVariables;
}

MappedMethods DataflowPass::getExpectedResult() { return this->expectedResult; }