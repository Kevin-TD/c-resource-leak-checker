#include "DataflowPass.h"
#include "Debug.h"
#include "TestRunner.h"
#include "Utils.h"

void DataflowPass::setFunctions(
    std::set<std::string> safeFunctions, std::set<std::string> reallocFunctions,
    std::map<std::string, std::string> memoryFunctions,
    AnnotationHandler annotations) {
  this->safeFunctions = safeFunctions;
  this->reallocFunctions = reallocFunctions;
  this->memoryFunctions = memoryFunctions;
  this->annotations = annotations;
}

void DataflowPass::setExpectedResult(FullProgram expectedResult) {
  this->expectedResult = expectedResult;
}

ProgramFunction DataflowPass::generatePassResults() {
  ProgramFunction PreProgramFunction;
  ProgramFunction PostProgramFunction;
  this->analyzeCFG(this->cfg, PreProgramFunction, PostProgramFunction, "");
  return PostProgramFunction;
}

void DataflowPass::setCFG(CFG *cfg) { this->cfg = cfg; }

void DataflowPass::transfer(
    Instruction *instruction, SetVector<Instruction *> workSet,
                ProgramPoint& inputProgramPoint) {
  
  std::string branchName = instruction->getParent()->getName().str();
  ProgramPoint programPoint = this->programFunction.getProgramPoint(branchName);

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

          std::string fnName = Call->getCalledFunction()->getName().str();
          ProgramVariable assignedVar = ProgramVariable(Store->getOperand(1));
          
          std::set<std::string> allAliases =
                programPoint.findVarAndNamedAliases(assignedVar.getCleanedName()); 
          logout("test " << *Store->getOperand(1) << " and "
                         << assignedVar.getCleanedName()) for (auto a :
                                                               allAliases) {
            logout("store alias = " << a)
          }
          
          
          if (this->memoryFunctions[fnName].size() > 0) {
            for (std::string alias : allAliases) {
              MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(alias)->getMethodsSetRef();

              logout("calling on alloc function for argname "
                     << alias << " and fnname " << fnName
                     << " fnname = " << fnName) this
                  ->onAllocationFunctionCall(*methods,
                                             this->memoryFunctions[fnName]);
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
                programPoint.findVarAndNamedAliases(
                    bitcastVar.getCleanedName());
            for (auto a : allAliases) {
              logout("bitcast alias = " << a)
            }

            if (this->memoryFunctions[fnName].size() > 0) {
              for (auto alias : allAliases) {
                MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(alias)->getMethodsSetRef();

                this->onAllocationFunctionCall(*methods,
                                               this->memoryFunctions[fnName]);
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
      ProgramVariable argumentVar = ProgramVariable(Call->getArgOperand(i));
      logout("cleaned name = " << argumentVar.getCleanedName())
          std::set<std::string>
              allAliases = programPoint.findVarAndNamedAliases(
                  argumentVar.getCleanedName());
      for (auto a : allAliases) {
        logout("argumentVar alias = " << a)
      }

      /*
      handles the case where function being called is "an indirect function
      invocation", meaning its target is determined at runtime. we are not
      running the original code, so we will treat it as an unkown function Docs:
      https://www.few.vu.nl/~lsc300/LLVM/doxygen/classllvm_1_1CallInst.html#a0bcd4131e1a1d92215f5385b4e16cd2e
      */
      if (Call->getCalledFunction() == NULL) {
        const DebugLoc &debugLoc = instruction->getDebugLoc();
        std::string location = "Line " + std::to_string(debugLoc.getLine()) +
                               ", Col " + std::to_string(debugLoc.getCol());

        errs() << "WARNING: implicitly declared function call on " << location
               << "\n";

        for (std::string aliasArg : allAliases) {
          MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(aliasArg)->getMethodsSetRef();
          this->onUnknownFunctionCall(*methods);
        }

        continue;
      }

      std::string fnName = Call->getCalledFunction()->getName().str();

      logout("call fnname = " << fnName)

          if (this->reallocFunctions.count(fnName)) {
        for (std::string aliasArg : allAliases) {
          MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(aliasArg)->getMethodsSetRef();
          this->onReallocFunctionCall(*methods, fnName);
        }
        continue;
      }

      if (this->safeFunctions.count(fnName)) {
        for (std::string aliasArg : allAliases) {
          MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(aliasArg)->getMethodsSetRef();
          this->onSafeFunctionCall(*methods, fnName);
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
            MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(aliasArg)->getMethodsSetRef();
            this->onDeallocationFunctionCall(*methods, fnName);
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

          // annotation reasoning goes here ...

          // if no annotations, treat it as unknown function
        for (std::string aliasArg : allAliases) {
          logout("pre get " << aliasArg)
          MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(aliasArg)->getMethodsSetRef();
          logout("post got")
          this->onUnknownFunctionCall(*methods);
          logout("post call")
        }
    }
  } else if (AllocaInst *allocate = dyn_cast<AllocaInst>(instruction)) {

    // searches for struct annotations
    if (llvm::StructType *structType =
            llvm::dyn_cast<llvm::StructType>(allocate->getAllocatedType())) {
      std::string structName = structType->getName();
      structName = dataflow::sliceString(
          structName, structName.find_last_of('.') + 1, structName.size() - 1);
      int numFields = structType->getNumElements();
      for (int i = 0; i < numFields; i++) {
        Annotation *anno = this->annotations.getStructAnnotation(
            structName, std::to_string(i));
        if (StructAnnotation *structAnno =
                dynamic_cast<StructAnnotation *>(anno)) {
          std::set<std::string> structMethods =
              structAnno->getAnnotationMethods();
          AnnotationType structAnnoType = structAnno->getAnnotationType();
          ProgramVariable sourceVar = ProgramVariable(allocate, i);

          std::set<std::string> aliases =
              inputProgramPoint.findVarAndNamedAliases(
                  sourceVar.getCleanedName());

          logout("found annotation " << dataflow::setToString(structMethods)
                                     << " "
                                     << annotationTypeToString(structAnnoType)
                                     << " for field index " << i << " var name "
                                     << sourceVar.getRawName())

              for (std::string alias : aliases) {
            for (std::string methodName : structMethods) {
              MethodsSet* methods = inputProgramPoint.getProgramVariableByCleanedNameRef(methodName)->getMethodsSetRef();
              logout("alias = '"
                     << alias << "' method name = '" << methodName
                     << "'") this->onAnnotation(*methods,
                                                methodName, structAnnoType);
            }
          }
        }
      }
    }
  }
}

void DataflowPass::analyzeCFG(CFG *cfg, ProgramFunction &PreProgramFunction,
                  ProgramFunction &PostProgramFunction, std::string priorBranch) {
  std::string currentBranch = cfg->getBranchName();

  if (currentBranch == "entry") {
    PreProgramFunction.addProgramPoint(ProgramPoint(currentBranch));
    llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

    
    ProgramPoint postProgramPoint = PostProgramFunction.getProgramPoint(currentBranch); 

    for (Instruction *instruction : instructions) {
      transfer(instruction, instructions, postProgramPoint);
    }

    logout("@DFP point name " << postProgramPoint.getName());
    for (auto var : postProgramPoint.getProgramVariables()) {
      logout("var name " << var.getCleanedName())
      std::set<std::string> methods = var.getMethodsSet().getMethods(); 
      logout("methods set " << dataflow::setToString(methods))
      for (auto alias : var.getAllAliases(false)) {
        logout(">>alias " << alias)
      }
    }

    for (CFG *succ : cfg->getSuccessors()) {
      analyzeCFG(succ, PreProgramFunction, PostProgramFunction, currentBranch);
    }

  }

  else {

    ProgramPoint PriorPrePoint = PreProgramFunction.getProgramPoint(currentBranch);
    ProgramPoint PriorPostPoint = PostProgramFunction.getProgramPoint(currentBranch);

    
    if (PriorPrePoint.getProgramVariables().size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch)

      ProgramPoint CurrentPrePoint = PostProgramFunction.getProgramPoint(priorBranch);

      // check if inputs (pre) differ
      if (CurrentPrePoint.equals(PriorPrePoint)) {
        return;
      }

      // lub PriorPreCM and CurrentPreCM
      ProgramPoint lub(currentBranch);

      std::list<ProgramVariable> priorPreVars = PriorPrePoint.getProgramVariables();
      for (ProgramVariable pv : priorPreVars) {
        MethodsSet lubSet;
        Value* pvVal = pv.getValue(); 

        MethodsSet priorPreMethodsSet = pv.getMethodsSet();
        MethodsSet currentPreMethodsSet = CurrentPrePoint.getProgramVariableByValue(pvVal).getMethodsSet();

        this->leastUpperBound(priorPreMethodsSet, currentPreMethodsSet, lubSet);

        ProgramVariable lubPV = ProgramVariable(pvVal, lubSet);
        lub.addVariable(lubPV);
      }

      // fill the lub with remaining facts from PriorPostPoint 
      std::list<ProgramVariable> priorPostVars = PriorPostPoint.getProgramVariables();
      for (ProgramVariable pv : priorPostVars) {
        Value* pvVal = pv.getValue(); 
        MethodsSet methods = pv.getMethodsSet();
        if (lub.getProgramVariableByValue(pvVal).getMethodsSet().isUninit()) {
          ProgramVariable priorPostVar = ProgramVariable(pvVal, methods);
          lub.addVariable(priorPostVar);
        }
      }

      PreProgramFunction.setProgramPoint(currentBranch, lub);

      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();
      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, lub);
      }

      PostProgramFunction.setProgramPoint(currentBranch, lub); 

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreProgramFunction, PostProgramFunction, currentBranch);
      }

    } else {
      logout("doing normal flow for " << currentBranch
                                      << " and prior = " << priorBranch)
      
      ProgramPoint priorPostPoint = PostProgramFunction.getProgramPoint(priorBranch);

      PreProgramFunction.getProgramPoint(currentBranch).setProgramVariables(priorPostPoint.getProgramVariables());

      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

      ProgramPoint flowInto = PostProgramFunction.getProgramPoint(priorBranch);

      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, flowInto);
      }

      PostProgramFunction.getProgramPoint(currentBranch).setProgramVariables(flowInto.getProgramVariables());

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreProgramFunction, PostProgramFunction, currentBranch);
      }
    }
  }
}

void DataflowPass::setAnnotations(AnnotationHandler annotations) {
  this->annotations = annotations;
}

FullProgram DataflowPass::getExpectedResult() {
  return this->expectedResult;
}

void DataflowPass::setProgramFunction(ProgramFunction programFunction) {
  this->programFunction = programFunction; 
}