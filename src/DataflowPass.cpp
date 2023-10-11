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

void DataflowPass::setExpectedResult(FullFile expectedResult) {
  this->expectedResult = expectedResult;
}

ProgramFunction DataflowPass::generatePassResults() {
  ProgramFunction preProgramFunction;
  ProgramFunction postProgramFunction;
  this->analyzeCFG(this->cfg, preProgramFunction, postProgramFunction, "");
  return postProgramFunction;
}

void DataflowPass::setCFG(CFG *cfg) { this->cfg = cfg; }

void DataflowPass::transfer(Instruction *instruction,
                            SetVector<Instruction *> workSet,
                            ProgramPoint &inputProgramPoint) {

  std::string branchName = instruction->getParent()->getName().str();

  if (auto Store = dyn_cast<StoreInst>(instruction)) {
    Value *valueToStore = Store->getOperand(0);
    Value *receivingValue = Store->getOperand(1);

    logout("\nvalue to store = " << *valueToStore);
    logout("receiving value = " << *receivingValue);

    for (auto Inst : workSet) {
      if (valueToStore == Inst) {
        if (auto Call = dyn_cast<CallInst>(Inst)) {
          /*
            For some x = malloc(params ...), we are only considering tracking x
            (argName)
          */

          std::string fnName = Call->getCalledFunction()->getName().str();
          ProgramVariable assignedVar = ProgramVariable(Store->getOperand(1));
          std::string arg = assignedVar.getCleanedName();
          MethodsSet *methods =
              inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();

          if (this->memoryFunctions[fnName].size() > 0 &&
              assignedVar.isIdentifier()) {

            logout("calling on alloc function for argname "
                   << arg << " and fnname " << fnName
                   << " fnname = " << fnName);
            this->onAllocationFunctionCall(methods,
                                           this->memoryFunctions[fnName]);
          } else if (ReturnAnnotation *returnAnno =
                         dynamic_cast<ReturnAnnotation *>(
                             this->annotations.getReturnAnnotation(fnName))) {
            logout("found return annnotation "
                   << returnAnno->generateStringRep());
            AnnotationType annoType = returnAnno->getAnnotationType();
            std::set<std::string> annoMethods =
                returnAnno->getAnnotationMethods();
            MethodsSet *argMethods =
                inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();

            for (std::string annoMethod : annoMethods) {
              this->onAnnotation(argMethods, annoMethod, annoType);
            }
          }

          break;
        } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(Inst)) {
          std::string argName = dataflow::variable(Store->getOperand(1));
          logout("arg name store inst bitcast = " << argName);

          if (argName[0] == '@') {
            argName[0] = '%';
          }

          if (CallInst *call = dyn_cast<CallInst>(bitcast->getOperand(0))) {
            std::string fnName = call->getCalledFunction()->getName().str();

            logout("fnname store inst bitcast = " << fnName);

            ProgramVariable bitcastVar = ProgramVariable(Store->getOperand(1));

            if (this->memoryFunctions[fnName].size() > 0 &&
                bitcastVar.isIdentifier()) {
              std::string arg = bitcastVar.getCleanedName();
              MethodsSet *methods =
                  inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
              this->onAllocationFunctionCall(methods,
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
      running the original code, so we will treat it as an unknown function
      Docs:
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
        this->onUnknownFunctionCall(methods);
        continue;
      }

      std::string fnName = Call->getCalledFunction()->getName().str();

      logout("call fnname = " << fnName);
      if (this->reallocFunctions.count(fnName)) {

        MethodsSet *methods =
            inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
        this->onReallocFunctionCall(methods, fnName);
        continue;
      }

      if (this->safeFunctions.count(fnName)) {
        MethodsSet *methods =
            inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
        this->onSafeFunctionCall(methods, fnName);
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
          logout("not calling onalloc for " << arg << " of fnName " << fnName
                                            << " and inst " << *instruction
                                            << " and just returning now");
          return;

        } else if (fnName == Pair.second) {
          MethodsSet *methods =
              inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
          logout("GETMAIN-5 ON '" << arg << "'");
          this->onDeallocationFunctionCall(methods, fnName);
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

      // finds annotations
      logout("finding annotations for " << fnName);

      logout("arg " << arg << " index " << i);

      Annotation *mayParameterAnnotation =
          this->annotations.getParameterAnnotation(fnName, i);
      if (ParameterAnnotation *paramAnno =
              dynamic_cast<ParameterAnnotation *>(mayParameterAnnotation)) {
        logout("found param annnotation " << paramAnno->generateStringRep());

        AnnotationType annoType = paramAnno->getAnnotationType();
        std::set<std::string> annoMethods = paramAnno->getAnnotationMethods();

        MethodsSet *argMethods =
            inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();

        for (std::string annoMethod : annoMethods) {
          this->onAnnotation(argMethods, annoMethod, annoType);
        }

        continue;
      }

      // no annotations found, treat function call as unknown function
      logout("no annotations found for " << fnName);
      MethodsSet *methods =
          inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
      this->onUnknownFunctionCall(methods);
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

        Annotation *anno = this->annotations.getStructAnnotation(structName, i);
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

          logout("found annotation "
                 << dataflow::setToString(structMethods) << " "
                 << annotationTypeToString(structAnnoType)
                 << " for field index " << std::to_string(i) << " var name "
                 << sourceVar.getRawName());

          MethodsSet *methods =
              inputProgramPoint.getPVRef(arg, false)->getMethodsSetRef();
          for (std::string methodName : structMethods) {
            logout("var = '" << arg << "' method name = '" << methodName
                             << "'");
            this->onAnnotation(methods, methodName, structAnnoType);
          }
        }
      }
    }
  }
}

void DataflowPass::analyzeCFG(CFG *cfg, ProgramFunction &preProgramFunction,
                              ProgramFunction &postProgramFunction,
                              std::string priorBranch) {
  std::string currentBranch = cfg->getBranchName();

  if (currentBranch == "entry") {
    preProgramFunction.addProgramPoint(ProgramPoint(currentBranch));
    llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

    ProgramPoint postProgramPoint =
        this->programFunction.getProgramPoint(currentBranch, true);

    for (Instruction *instruction : instructions) {
      transfer(instruction, instructions, postProgramPoint);
    }

    postProgramFunction.addProgramPoint(postProgramPoint);

    for (CFG *succ : cfg->getSuccessors()) {
      analyzeCFG(succ, preProgramFunction, postProgramFunction, currentBranch);
    }

  }

  else {

    ProgramPoint *priorPrePoint =
        preProgramFunction.getProgramPointRef(currentBranch, true);
    ProgramPoint *priorPostPoint =
        postProgramFunction.getProgramPointRef(currentBranch, true);

    priorPostPoint->add(
        this->programFunction.getProgramPointRef(currentBranch, true));

    if (priorPrePoint->getProgramVariables().size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch);

      ProgramPoint *currentPrePoint =
          postProgramFunction.getProgramPointRef(priorBranch, true);

      // check if inputs (pre) differ
      if (currentPrePoint->equals(priorPrePoint)) {
        return;
      }

      // lub PriorPreCM and CurrentPreCM
      ProgramPoint lub(currentBranch);

      std::list<ProgramVariable> priorPreVars =
          priorPrePoint->getProgramVariables();

      for (ProgramVariable pv : priorPreVars) {
        MethodsSet lubMethodsSet;

        MethodsSet priorPreMethodsSet = pv.getMethodsSet();

        MethodsSet currentPreMethodsSet;
        if (currentPrePoint->varExists(pv.getCleanedName())) {
          currentPreMethodsSet.setMethods(
              currentPrePoint->getPVRef(pv.getCleanedName(), false)
                  ->getMethodsSet()
                  .getMethods());
        }

        this->leastUpperBound(priorPreMethodsSet, currentPreMethodsSet,
                              lubMethodsSet);

        pv.setMethodsSet(lubMethodsSet);

        lub.addVariable(pv);
      }

      // fill the lub with remaining facts from priorPostPoint
      lub.add(priorPostPoint);

      preProgramFunction.setProgramPoint(currentBranch, lub);

      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();
      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, lub);
      }

      postProgramFunction.setProgramPoint(currentBranch, lub);

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, preProgramFunction, postProgramFunction,
                   currentBranch);
      }

    } else {
      logout("doing normal flow for " << currentBranch
                                      << " and prior = " << priorBranch);

      ProgramPoint *priorPostPoint =
          postProgramFunction.getProgramPointRef(priorBranch, true);

      preProgramFunction.getProgramPointRef(currentBranch, true)
          ->setProgramVariables(priorPostPoint);

      llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

      ProgramPoint flowInto = ProgramPoint(
          currentBranch,
          postProgramFunction.getProgramPointRef(priorBranch, true));

      flowInto.add(
          this->programFunction.getProgramPointRef(currentBranch, true));

      for (Instruction *instruction : instructions) {
        transfer(instruction, instructions, flowInto);
      }

      postProgramFunction.getProgramPointRef(currentBranch, true)
          ->setProgramVariables(flowInto);

      for (CFG *succ : cfg->getSuccessors()) {
        analyzeCFG(succ, preProgramFunction, postProgramFunction,
                   currentBranch);
      }
    }
  }
}

void DataflowPass::setAnnotations(AnnotationHandler annotations) {
  this->annotations = annotations;
}

FullFile DataflowPass::getExpectedResult() { return this->expectedResult; }

void DataflowPass::setProgramFunction(ProgramFunction programFunction) {
  this->programFunction = programFunction;
}