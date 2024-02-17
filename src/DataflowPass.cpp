#include "DataflowPass.h"
#include "Constants.h"
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

void DataflowPass::setCFG(CFG *cfg) {
    this->cfg = cfg;
}

void DataflowPass::transfer(Instruction *instruction,
                            ProgramPoint &inputProgramPoint) {
    std::string branchName = instruction->getParent()->getName().str();

    if (StoreInst *store = dyn_cast<StoreInst>(instruction)) {
        Value *valueToStore = store->getOperand(0);
        Value *receivingValue = store->getOperand(1);

        logout("\nvalue to store = " << *valueToStore);
        logout("receiving value = " << *receivingValue);

        if (CallInst *call = dyn_cast<CallInst>(valueToStore)) {
            logout("storing value is call inst");
            /*
              For some x = malloc(params ...), we are only considering tracking x
              (argName)
            */

            std::string fnName = call->getCalledFunction()->getName().str();
            ProgramVariable assignedVar = ProgramVariable(store->getOperand(1));
            std::string arg = assignedVar.getCleanedName();
            PVAliasSet *pvas = inputProgramPoint.getPVASRef(assignedVar, false);

            if (this->memoryFunctions[fnName].size() > 0 &&
                    assignedVar.isIdentifier()) {

                logout("calling on alloc function for argname "
                       << arg << " and fnname " << fnName << " fnname = " << fnName);
                this->onAllocationFunctionCall(pvas, this->memoryFunctions[fnName]);
            } else if (ReturnAnnotation *returnAnno =
                           dynamic_cast<ReturnAnnotation *>(
                               this->annotations.getReturnAnnotation(fnName))) {
                logout("found return annotation " << returnAnno->generateStringRep());
                // "a"
                this->insertAnnotation(returnAnno, pvas);
            } else if (pvas->containsStructFieldVar()) {
                if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(
                                                       this->annotations.getReturnAnnotation(fnName,
                                                               pvas->getIndex()))) {
                    // "b"
                    this->insertAnnotation(returnAnno, pvas);
                }
            }

        } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(valueToStore)) {
            std::string argName = rlc_dataflow::variable(store->getOperand(1));
            logout("arg name store inst bitcast = " << argName);

            if (argName[0] == '@') {
                argName[0] = '%';
            }

            if (CallInst *call = dyn_cast<CallInst>(bitcast->getOperand(0))) {
                std::string fnName = call->getCalledFunction()->getName().str();

                logout("fnname store inst bitcast = " << fnName);

                ProgramVariable bitcastVar = ProgramVariable(store->getOperand(1));

                if (this->memoryFunctions[fnName].size() > 0 &&
                        bitcastVar.isIdentifier()) {
                    std::string arg = bitcastVar.getCleanedName();
                    PVAliasSet* pvas = inputProgramPoint.getPVASRef(bitcastVar, false);
                    this->onAllocationFunctionCall(pvas,
                                                   this->memoryFunctions[fnName]);
                }
            }
        } else if (ExtractValueInst *extractValue =
                       dyn_cast<ExtractValueInst>(valueToStore)) {
            /*
            extractvalue instructions specifies a field member. We check to see if
            the value it's extracting from is a function.

            Docs: https://llvm.org/docs/LangRef.html#extractvalue-instruction
            */
            ProgramVariable assignedVar = ProgramVariable(extractValue);
            std::string arg = assignedVar.getCleanedName();
            PVAliasSet *pvas = inputProgramPoint.getPVASRef(assignedVar, false);

            if (!pvas->containsStructFieldVar()) {
                return;
            }

            logout("found pv " << pvas->toString(true)
                   << " with index " << pvas->getIndex());

            if (CallInst *call = dyn_cast<CallInst>(extractValue->getOperand(0))) {
                std::string fnName = call->getCalledFunction()->getName().str();
                int fieldIndex = extractValue->getIndices()[0];

                logout("traced back extract value to " << fnName);
                logout("index specified = " << fieldIndex);

                if (fieldIndex != pvas->getIndex()) {
                    return;
                }

                if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(
                                                       this->annotations.getReturnAnnotation(fnName, fieldIndex))) {
                    logout("found annotation from extract value "
                           << returnAnno->generateStringRep());
                    // "c"
                    this->insertAnnotation(returnAnno, pvas);
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

    } else if (CallInst *call = dyn_cast<CallInst>(instruction)) {
        for (unsigned i = 0; i < call->getNumArgOperands(); ++i) {
            ProgramVariable argumentVar = ProgramVariable(call->getArgOperand(i));
            std::string arg = argumentVar.getCleanedName();
            PVAliasSet *pvas = inputProgramPoint.getPVASRef(argumentVar, false);

            if (!argumentVar.isIdentifier()) {
                continue;
            }

            if (handleIfKnownFunctionForCallInsts(call, pvas)) {
                continue;
            }

            std::string fnName = call->getCalledFunction()->getName().str();

            if (rlc_util::startsWith(fnName, LLVM_PTR_ANNOTATION) ||
                    rlc_util::startsWith(fnName, LLVM_VAR_ANNOTATION)) {
                return;
            }

            if (handleSretCallForCallInsts(call, i, fnName, arg, inputProgramPoint)) {
                return;
            }

            if (handleIfAnnotationExistsForCallInsts(fnName, i, pvas)) {
                continue;
            }

            // no annotations found, treat function call as unknown function
            logout("no annotations found for " << fnName << " index " << i << " | pvas = " << pvas->toString(false));
            // "p"
            std::string t = fnName; // TODO revert back to fnName
            this->onUnknownFunctionCall(pvas, t );
        }
    } else if (AllocaInst *allocate = dyn_cast<AllocaInst>(instruction)) {

    }
}

void DataflowPass::analyzeCFG(CFG *cfg, ProgramFunction &preProgramFunction,
                              ProgramFunction &postProgramFunction,
                              std::string priorBranch) {
    std::string currentBranch = cfg->getBranchName();

    if (currentBranch == FIRST_BRANCH_NAME) {
        preProgramFunction.addProgramPoint(ProgramPoint(currentBranch));
        llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

        ProgramPoint postProgramPoint =
            this->programFunction.getProgramPoint(currentBranch, true);

        for (Instruction *instruction : instructions) {
            transfer(instruction, postProgramPoint);
        }

        postProgramFunction.addProgramPoint(postProgramPoint);

        for (CFG *succ : cfg->getSuccessors()) {
            analyzeCFG(succ, preProgramFunction, postProgramFunction, currentBranch);
        }

        return;
    }

    ProgramPoint *priorPrePoint =
        preProgramFunction.getProgramPointRef(currentBranch, true);
    ProgramPoint *priorPostPoint =
        postProgramFunction.getProgramPointRef(currentBranch, true);

    priorPostPoint->add(
        this->programFunction.getProgramPointRef(currentBranch, true));

    if (priorPrePoint->getProgramVariableAliasSets().size() > 0) {
        logout("need to lub for " << currentBranch << " " << priorBranch);

        ProgramPoint *currentPrePoint =
            postProgramFunction.getProgramPointRef(priorBranch, true);

        // check if inputs (pre) differ
        if (currentPrePoint->equals(priorPrePoint)) {
            return;
        }

        // lub PriorPreCM and CurrentPreCM
        ProgramPoint lub(currentBranch);

        DisjointPVAliasSets priorPreVars =
            priorPrePoint->getProgramVariableAliasSets();

        for (PVAliasSet pvas : priorPreVars.getSets()) {
            MethodsSet lubMethodsSet;

            MethodsSet priorPreMethodsSet = pvas.getMethodsSet();

            MethodsSet curPrePointMethods;
            for (ProgramVariable pv : pvas.getProgramVariables()) {
                if (currentPrePoint->varExists(pv)) {
                    curPrePointMethods = currentPrePoint->getPVASRef(pv, false)->getMethodsSet();
                    break;
                }
            }

            this->leastUpperBound(pvas, curPrePointMethods);

            lub.addPVAS(pvas);
        }

        // fill the lub with remaining facts from priorPostPoint
        lub.add(priorPostPoint);

        preProgramFunction.setProgramPoint(currentBranch, lub);

        llvm::SetVector<Instruction *> instructions = cfg->getInstructions();
        for (Instruction *instruction : instructions) {
            transfer(instruction, lub);
        }

        postProgramFunction.setProgramPoint(currentBranch, lub);

        for (CFG *succ : cfg->getSuccessors()) {
            analyzeCFG(succ, preProgramFunction, postProgramFunction, currentBranch);
        }

    } else {
        logout("doing normal flow for " << currentBranch
               << " and prior = " << priorBranch);

        ProgramPoint *priorPostPoint =
            postProgramFunction.getProgramPointRef(priorBranch, true);

        preProgramFunction.getProgramPointRef(currentBranch, true)
        ->setProgramVariableAliasSets(priorPostPoint->getProgramVariableAliasSets());

        llvm::SetVector<Instruction *> instructions = cfg->getInstructions();

        ProgramPoint flowInto =
            ProgramPoint(currentBranch,
                         postProgramFunction.getProgramPointRef(priorBranch, true));

        flowInto.add(this->programFunction.getProgramPointRef(currentBranch, true));

        for (Instruction *instruction : instructions) {
            transfer(instruction, flowInto);
        }

        postProgramFunction.getProgramPointRef(currentBranch, true)
        ->setProgramVariableAliasSets(flowInto.getProgramVariableAliasSets());

        for (CFG *succ : cfg->getSuccessors()) {
            analyzeCFG(succ, preProgramFunction, postProgramFunction, currentBranch);
        }
    }
}

void DataflowPass::insertAnnotation(Annotation *anno, PVAliasSet *pvas, const std::string& invokerFnName) {
    AnnotationType annoType = anno->getAnnotationType();
    std::set<std::string> annoMethods = anno->getAnnotationMethods();

    for (std::string annoMethod : annoMethods) {
        this->onAnnotation(pvas, annoMethod, invokerFnName, annoType);
    }
}

void DataflowPass::insertAnnotation(Annotation *anno, PVAliasSet *pvas) {
    AnnotationType annoType = anno->getAnnotationType();
    std::set<std::string> annoMethods = anno->getAnnotationMethods();

    for (std::string annoMethod : annoMethods) {
        this->onAnnotation(pvas, annoMethod, annoType);
    }
}

bool DataflowPass::handleSretCallForCallInsts(CallInst *call, int argIndex,
        const std::string &fnName,
        const std::string &argName,
        ProgramPoint &programPoint) {
    if (argIndex != 0 || !call->getCalledFunction()->hasStructRetAttr()) {
        return false;
    }

    logout("func has sret");

    if (call->getArgOperand(0)->getType()->isPointerTy()) {
        Type *pointerType =
            call->getArgOperand(0)->getType()->getPointerElementType();
        if (pointerType->isStructTy()) {
            StructType *structType = cast<StructType>(pointerType);

            int numFields = structType->getNumElements();

            for (int fieldIndex = 0; fieldIndex < numFields; fieldIndex++) {
                std::string fieldArg = argName + "." + std::to_string(fieldIndex);
                PVAliasSet *pvasField = programPoint.getPVASRef(fieldArg, false);

                logout("found field " << pvasField->toString(true));

                if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(
                                                       this->annotations.getReturnAnnotation(fnName, fieldIndex))) {
                    logout("found annotation from sret "
                           << returnAnno->generateStringRep());
                    // "d"
                    this->insertAnnotation(returnAnno, pvasField);
                }
            }
        }
    }

    for (int j = 1; j < call->getNumArgOperands(); j++) {
        auto nextCallArg = call->getArgOperand(j);
        ProgramVariable nextArgVar = ProgramVariable(nextCallArg);
        std::string nextArg = nextArgVar.getCleanedName();

        if (nextCallArg->getType()->isPointerTy()) {
            Type *pointerType = nextCallArg->getType()->getPointerElementType();
            if (pointerType->isStructTy()) {
                StructType *structType = cast<StructType>(pointerType);

                int numFields = structType->getNumElements();

                for (int fieldIndex = 0; fieldIndex < numFields; fieldIndex++) {
                    std::string fieldArg = nextArg + "." + std::to_string(fieldIndex);
                    PVAliasSet *pvasField = programPoint.getPVASRef(fieldArg, false);

                    logout("found next arg fields "
                           << pvasField->toString(true)
                           << " for j = " << j);

                    auto allAnnotationsWithFields =
                        this->annotations.getAllParameterAnnotationsWithFields(fnName);
                    for (Annotation *anno : allAnnotationsWithFields) {
                        if (ParameterAnnotation *paramAnno =
                                    dynamic_cast<ParameterAnnotation *>(anno)) {
                            if (paramAnno->getField() == fieldIndex) {
                                logout("found param annotation for sret annos with fields "
                                       << paramAnno->generateStringRep());
                                // "e"
                                this->insertAnnotation(paramAnno, pvasField, fnName);
                            }
                        }
                    }
                }
            } else {
                // arg is not a struct type
                auto allAnnotationsWithoutFields =
                    this->annotations.getAllParameterAnnotationsWithoutFields(fnName);
                for (Annotation *anno : allAnnotationsWithoutFields) {
                    if (ParameterAnnotation *paramAnno =
                                dynamic_cast<ParameterAnnotation *>(anno)) {
                        // j - 1 because the 0th argument of the IR function call is not a
                        // part of the actual function. the rest of the arguments are the
                        // ones a part of the call.

                        if (paramAnno->getNthParameter() == j - 1) {
                            PVAliasSet *argVar = programPoint.getPVASRef(nextArg, false);
                            logout("found param annotation for j "
                                   << paramAnno->generateStringRep() << " for j = " << j
                                   << " and var "
                                   << argVar->toString(true));
                            // "f"
                            this->insertAnnotation(paramAnno, argVar, fnName);
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool DataflowPass::handleIfKnownFunctionForCallInsts(CallInst *call,
        PVAliasSet *pvas) {
    /*
    handles the case where function being called is "an indirect function
    invocation", meaning its target is determined at runtime. we are not
    running the original code, so we will treat it as an unknown function
    Docs:
    https://www.few.vu.nl/~lsc300/LLVM/doxygen/classllvm_1_1CallInst.html#a0bcd4131e1a1d92215f5385b4e16cd2e
    */
    std::string fnName = call->getCalledFunction()->getName().str();

    if (call->getCalledFunction() == NULL) {
        const DebugLoc &debugLoc = call->getDebugLoc();

        std::string location = "Line " + std::to_string(debugLoc.getLine()) +
                               ", Col " + std::to_string(debugLoc.getCol());

        errs() << "WARNING: implicitly declared function call on " << location
               << "\n";

        this->onUnknownFunctionCall(pvas, fnName);
        return true;
    }



    logout("call fnname = " << fnName);
    if (this->reallocFunctions.count(fnName)) {

        this->onReallocFunctionCall(pvas, fnName);
        return true;
    }

    if (this->safeFunctions.count(fnName)) {
        this->onSafeFunctionCall(pvas, fnName);
        return true;
    }

    // call void @free(i8* %7) #2, !dbg !291   <- args matter
    // %call5 = call noalias i8* @aligned_alloc(i64 %4, i64 %5) #2, !dbg !287
    // <- args do not matter
    /*
      we are making the following assumption:
      for deallocation functions, the arguments matter since they will contain
      the pointers/var names to memory. for allocation functions, the arguments
      do not matter since they are typically numbers. the variable that is
      receiving the memory is handled elswhere (in the handling of store
      insts)
    */

    for (auto allocDeallocPair : this->memoryFunctions) {
        std::string allocationFunction = allocDeallocPair.first;
        std::string deallocationFunction = allocDeallocPair.second;

        if (fnName == allocationFunction) {
            return true;
        } else if (fnName == deallocationFunction) {
            this->onDeallocationFunctionCall(pvas, fnName);
            return true;
        }
    }

    // llvm.dbg.declare are function calls made by the IR to set debug
    // information. this function does not have annotations, as it's a function
    // by the IR, not the C code. thus, we do not check it for annotations.
    if (fnName == "llvm.dbg.declare") {
        return true;
    }

    return false;
}

bool DataflowPass::handleIfAnnotationExistsForCallInsts(const std::string &fnName, int argIndex, PVAliasSet *pvas) {
    // checks for parameter annotations (no field specified)
    Annotation *mayParameterAnnotation =
        this->annotations.getParameterAnnotation(fnName, argIndex);
    if (ParameterAnnotation *paramAnno =
                dynamic_cast<ParameterAnnotation *>(mayParameterAnnotation)) {
        logout("found param annotation " << paramAnno->generateStringRep());
        // "g"
        this->insertAnnotation(paramAnno, pvas, fnName);
        return true;
    }

    // checks for parameter annotations (field specified)
    if (pvas->containsStructFieldVar()) {
        Annotation *mayParamAnnoWithField =
            this->annotations.getParameterAnnotation(fnName, argIndex,
                    pvas->getIndex());
        if (ParameterAnnotation *paramAnno =
                    dynamic_cast<ParameterAnnotation *>(mayParamAnnoWithField)) {
            logout("found param annotation for struct "
                   << paramAnno->generateStringRep());
            // "h"
            this->insertAnnotation(paramAnno, pvas, fnName);
            return true;
        }
    }

    return false;
}

void DataflowPass::setAnnotations(AnnotationHandler annotations) {
    this->annotations = annotations;
}

FullFile DataflowPass::getExpectedResult() {
    return this->expectedResult;
}

void DataflowPass::setProgramFunction(ProgramFunction programFunction) {
    this->programFunction = programFunction;
}