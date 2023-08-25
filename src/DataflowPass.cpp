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

void DataflowPass::transfer(Instruction *instruction,
                            SetVector<Instruction *> workSet,
                            ProgramPoint &inputProgramPoint) {

  std::string branchName = instruction->getParent()->getName().str();

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

          if (this->memoryFunctions[fnName].size() > 0 &&
              assignedVar.isIdentifier()) {
            std::string arg = assignedVar.getCleanedName();
            MethodsSet *methods =
                inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();

            logout("calling on alloc function for argname "
                   << arg << " and fnname " << fnName
                   << " fnname = " << fnName) this
                ->onAllocationFunctionCall(*methods,
                                           this->memoryFunctions[fnName]);
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

            if (this->memoryFunctions[fnName].size() > 0 &&
                bitcastVar.isIdentifier()) {
              std::string arg = bitcastVar.getCleanedName();
              MethodsSet *methods =
                  inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
              this->onAllocationFunctionCall(*methods,
                                             this->memoryFunctions[fnName]);

              break;
            }
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
      std::string arg = argumentVar.getCleanedName();

      if (!argumentVar.isIdentifier()) {
        continue;
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

        MethodsSet *methods =
            inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
        this->onUnknownFunctionCall(*methods);
        continue;
      }

      std::string fnName = Call->getCalledFunction()->getName().str();

      logout("call fnname = " << fnName)

          if (this->reallocFunctions.count(fnName)) {

        MethodsSet *methods =
            inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
        this->onReallocFunctionCall(*methods, fnName);
        continue;
      }

      if (this->safeFunctions.count(fnName)) {
        MethodsSet *methods =
            inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
        this->onSafeFunctionCall(*methods, fnName);
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
          logout("not calling onalloc for "
                 << arg << " of fnName " << fnName << " and inst "
                 << *instruction << " and just returning now") return;

        } else if (fnName == Pair.second) {
          MethodsSet *methods =
              inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
          logout("GETMAIN-5 ON '"
                 << arg
                 << "'") this->onDeallocationFunctionCall(*methods, fnName);
          loopBroken = true;
          break;
        }
      }

      if (loopBroken) {
        continue;
      }

      if (fnName == "llvm.dbg.declare") {
        continue;
      }

      // only cases here are calls to functions not already seen before

      logout("also unknown function call " << fnName)

          // TODO: annotation reasoning goes here

          // if no annotations, treat it as unknown function
          MethodsSet *methods =
              inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
      logout("GETMAIN-6 ON '" << arg
                              << "'") this->onUnknownFunctionCall(*methods);
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

          std::string arg = sourceVar.getCleanedName();
          if (!sourceVar.isIdentifier()) {
            continue;
          }

          logout("found annotation " << dataflow::setToString(structMethods)
                                     << " "
                                     << annotationTypeToString(structAnnoType)
                                     << " for field index " << i << " var name "
                                     << sourceVar.getRawName())

              for (std::string methodName : structMethods) {
            MethodsSet *methods =
                inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
            logout("var = '" << arg << "' method name = '" << methodName
                             << "'") this->onAnnotation(*methods, methodName,
                                                        structAnnoType);
          }
        }
      }
    }
  }
}

void DataflowPass::analyzeCFG(CFG *cfg, ProgramFunction &PreProgramFunction,
                              ProgramFunction &PostProgramFunction,
                              std::string priorBranch) {
  std::string currentBranch = cfg->getBranchName();

  if (currentBranch == "entry") {
    PreProgramFunction.addProgramPoint(ProgramPoint(currentBranch));
    llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

    ProgramPoint postProgramPoint =
        this->programFunction.getProgramPoint(currentBranch, true);

    for (Instruction *instruction : instructions) {
      transfer(instruction, instructions, postProgramPoint);
    }

    PostProgramFunction.addProgramPoint(postProgramPoint);

    for (CFG *succ : cfg->getSuccessors()) {
      analyzeCFG(succ, PreProgramFunction, PostProgramFunction, currentBranch);
    }

  }

  else {

    ProgramPoint *PriorPrePoint =
        PreProgramFunction.getProgramPointRef(currentBranch, true);
    ProgramPoint *PriorPostPoint =
        PostProgramFunction.getProgramPointRef(currentBranch, true);

    PriorPostPoint->add(
        this->programFunction.getProgramPoint(currentBranch, true));

    if (PriorPrePoint->getProgramVariables().size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch)

          ProgramPoint *CurrentPrePoint =
              PostProgramFunction.getProgramPointRef(priorBranch, true);

      // check if inputs (pre) differ
      if (CurrentPrePoint->equals(PriorPrePoint)) {
        return;
      }

      // lub PriorPreCM and CurrentPreCM
      ProgramPoint lub(currentBranch);

      std::list<ProgramVariable> priorPreVars =
          PriorPrePoint->getProgramVariables();

      for (ProgramVariable pv : priorPreVars) {
        std::set<std::string> lubSet;

        std::set<std::string> priorPreSet = pv.getMethodsSet().getMethods();
        std::set<std::string> currentPreSet =
            CurrentPrePoint->getPVRef(pv.getCleanedName(), false)
                ->getMethodsSet()
                .getMethods();

        this->leastUpperBound(priorPreSet, currentPreSet, lubSet);

        MethodsSet lubMethodsSet = MethodsSet(lubSet);
        pv.setMethodsSet(lubMethodsSet);

        lub.addVariable(pv);
      }

      // fill the lub with remaining facts from PriorPostPoint
      lub.add(*PriorPostPoint);

      PreProgramFunction.setProgramPoint(currentBranch, lub);

      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();
      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, lub);
      }

      PostProgramFunction.setProgramPoint(currentBranch, lub);

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreProgramFunction, PostProgramFunction,
                   currentBranch);
      }

    } else {
      logout("doing normal flow for " << currentBranch
                                      << " and prior = " << priorBranch)

          ProgramPoint *priorPostPoint =
              PostProgramFunction.getProgramPointRef(priorBranch, true);

      PreProgramFunction.getProgramPointRef(currentBranch, true)
          ->setProgramVariables(priorPostPoint);

      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

      ProgramPoint flowInto = ProgramPoint(
          currentBranch,
          PostProgramFunction.getProgramPointRef(priorBranch, true));

      flowInto.add(this->programFunction.getProgramPoint(currentBranch, true));

      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, flowInto);
      }

      PostProgramFunction.getProgramPointRef(currentBranch, true)
          ->setProgramVariables(flowInto);

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreProgramFunction, PostProgramFunction,
                   currentBranch);
      }
    }
  }
}

void DataflowPass::setAnnotations(AnnotationHandler annotations) {
  this->annotations = annotations;
}

FullProgram DataflowPass::getExpectedResult() { return this->expectedResult; }

void DataflowPass::setProgramFunction(ProgramFunction programFunction) {
  this->programFunction = programFunction;
}