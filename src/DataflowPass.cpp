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
                              // TODO: remove workset from arg list
  std::string branchName = instruction->getParent()->getName().str();

  if (auto Store = dyn_cast<StoreInst>(instruction)) {
    Value *valueToStore = Store->getOperand(0);
    Value *receivingValue = Store->getOperand(1);

    logout("\nvalue to store = " << *valueToStore);
    logout("receiving value = " << *receivingValue);

    if (auto Call = dyn_cast<CallInst>(valueToStore)) {
      logout("storing value is call inst");
      /*
        For some x = malloc(params ...), we are only considering tracking x
        (argName)
      */

      std::string fnName = Call->getCalledFunction()->getName().str();
      ProgramVariable assignedVar = ProgramVariable(Store->getOperand(1));
      std::string arg = assignedVar.getCleanedName();
      ProgramVariable* pv = inputProgramPoint.getPVRef(arg, false); 
      MethodsSet *methods =
          pv->getMethodsSetRef();
      

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
        logout("found return annotation "
                << returnAnno->generateStringRep());
        this->insertAnnotation(returnAnno, pv);
      }  else if (pv->hasIndex()) {
          if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(this->annotations.getReturnAnnotation(fnName, pv->getIndex()))) {
            this->insertAnnotation(returnAnno, pv);
          }
      }

    } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(valueToStore)) {
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

        }
      }
    } else if (ExtractValueInst* extractValue = dyn_cast<ExtractValueInst>(valueToStore)) {
      /*
      extractvalue instructions specifies a field member. We check to see if
      the value it's extracting from is a function. 

      Docs: https://llvm.org/docs/LangRef.html#extractvalue-instruction
      */
      ProgramVariable assignedVar = ProgramVariable(extractValue);
      std::string arg = assignedVar.getCleanedName();
      ProgramVariable* pv = inputProgramPoint.getPVRef(arg, false); 

      if (!pv->hasIndex()) {
        return; 
      }

      logout("found pv " << pv->getCleanedName() << " with index " << pv->getIndex());
      
      if (CallInst* call = dyn_cast<CallInst>(extractValue->getOperand(0))) {
        std::string fnName = call->getCalledFunction()->getName().str();
        int fieldIndex = extractValue->getIndices()[0]; 

        logout("traced back extract value to " << fnName);
        logout("index specified = " << fieldIndex); 

        if (fieldIndex != pv->getIndex()) {
          return; 
        }

        if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(this->annotations.getReturnAnnotation(fnName, fieldIndex))) {
          logout("found annotation from extract value " << returnAnno->generateStringRep());
          this->insertAnnotation(returnAnno, pv);
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
      ProgramVariable* pv = inputProgramPoint.getPVRef(arg, false); 

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
            pv->getMethodsSetRef();
        this->onUnknownFunctionCall(methods);
        continue;
      }

      std::string fnName = Call->getCalledFunction()->getName().str();

      logout("call fnname = " << fnName);
      if (this->reallocFunctions.count(fnName)) {

        MethodsSet *methods =
            pv->getMethodsSetRef();
        this->onReallocFunctionCall(methods, fnName);
        continue;
      }

      if (this->safeFunctions.count(fnName)) {
        MethodsSet *methods =
            pv->getMethodsSetRef();
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
              pv->getMethodsSetRef();
          logout("GETMAIN-5 ON '" << arg << "'");
          this->onDeallocationFunctionCall(methods, fnName);
          loopBroken = true;
          break;
        }
      }

      if (loopBroken || fnName == "llvm.dbg.declare") {
        continue;
      }


      // finds annotations
      logout("finding annotations for " << fnName);

      if (pv == NULL) {
        /*
        LLVM uses sret attributes as an optimization if the return type is large. a struct with more than 2 fields can trigger this optimization. 

        docs: https://llvm.org/docs/LangRef.html#parameter-attributes 
        less credible forum post: https://discourse.llvm.org/t/optimizing-sret-on-caller-side/60660 
      
        our instructions end up looking like this: 
        call void @does_something_a(%struct.my_struct* sret %A1, %struct.my_struct* byval align 8 %A), !dbg !58

        pv ends up being null for the following reason: 
          lets say we have struct A with fields x, y, z.
          when the struct is initialized, we store A.0, A.1, 
          and A.2 as local program variables, but not A itself. 
          so, when we get to this function call, it's looking for 
          the variable A, but it does not exist.

          if LLVM does not use sret, this issue would not occur. 
          this is because there would be getelementptr instructions
          that would break apart our struct's fields into IR variables, so 
          when we call does_something_a, it would pass in variables 
          A.0 or A.1 instead of just A. 

        here's how we will handle these types of instructions 

        say we have the instruction 
        call void @does_something_a(%struct.my_struct* sret %A1, %struct.my_struct* byval align 8 %A), !dbg !58
        
        this is from C code 
        my_struct A1 = does_something_a(A); 

        *we assume LHS is always a struct and first argument always has sret attribute

        we will: 
        -destructure A1 into its fields (A1.0, A1.1, ...)
        -determine A1's MustCall/CalledMethods from annotations on return on does_something_a
        -destructure A into its fields (A.0, A.1, ...)
        -determine A's MustCall/CalledMethods from annotations on params on does_something a
        past the first argument, it could be struct or non-struct. non-structs will have their MustCall/CalledMethods determined, but without the destructuring
        */

        logout("arg " << arg << " index " << i << " null. assuming it's a special case with sret");

        if (Call->getCalledFunction()->hasStructRetAttr() && i == 0) {
          logout("func has sret"); 
          this->handleSretCall(Call, fnName, arg, inputProgramPoint); 
          return; 
        }
      }

      logout("arg " << arg << " index " << i << " " << pv->getCleanedName());
      
      // checks for parameter annotations (no field specified)
      Annotation *mayParameterAnnotation =
          this->annotations.getParameterAnnotation(fnName, i);
      if (ParameterAnnotation *paramAnno =
              dynamic_cast<ParameterAnnotation *>(mayParameterAnnotation)) {
        logout("found param annotation " << paramAnno->generateStringRep());
        this->insertAnnotation(paramAnno, pv);
        continue;
      }
      
      // checks for parameter annotations (field specified)
      if (pv->hasIndex()) {
        Annotation* mayParamAnnoWithField = this->annotations.getParameterAnnotation(fnName, i, pv->getIndex()); 
        if (ParameterAnnotation *paramAnno = dynamic_cast<ParameterAnnotation *>(mayParamAnnoWithField)) {
          logout("found param struct annotation " << paramAnno->generateStringRep());
          this->insertAnnotation(paramAnno, pv);
          continue;   
        }
      }
  
      // no annotations found, treat function call as unknown function
      logout("no annotations found for " << fnName);
      MethodsSet *methods = pv->getMethodsSetRef();
      this->onUnknownFunctionCall(methods);
    }
  } else if (AllocaInst *allocate = dyn_cast<AllocaInst>(instruction)) {

    // searches for struct annotations
    StructType* structType = llvm::dyn_cast<llvm::StructType>(allocate->getAllocatedType()); 

    if (!structType && allocate->getAllocatedType()->isPointerTy()) {
      Type *pointerType = allocate->getAllocatedType()->getPointerElementType();
      if (pointerType->isStructTy()) { 
        structType = cast<StructType>(pointerType);
      } else {
        return; 
      }
    }

    std::string structName = structType->getName();
    structName = dataflow::sliceString(
        structName, structName.find_last_of('.') + 1, structName.size() - 1);
    int numFields = structType->getNumElements();
    for (int i = 0; i < numFields; i++) {

      Annotation *anno = this->annotations.getStructAnnotation(structName, i);
      if (StructAnnotation *structAnno =
              dynamic_cast<StructAnnotation *>(anno)) {
        ProgramVariable sourceVar = ProgramVariable(allocate, i);

        if (!sourceVar.isIdentifier()) {
          continue;
        }

        logout("found annotation for struct " << structAnno->generateStringRep()); 

        std::string arg = sourceVar.getCleanedName();
        ProgramVariable* pv = inputProgramPoint.getPVRef(arg, false); 
        this->insertAnnotation(structAnno, pv); 

        // ProgramVariable structPV = ProgramVariable(allocate);
        // ProgramVariable* originalStructPVRef = inputProgramPoint.getPVRef(structPV.getCleanedName(), false); 
        // if (originalStructPVRef) {
        //   ProgramVariable structVar = ProgramVariable(originalStructPVRef->getValue(), i); 
        //   logout("(alloca inst) specifying index for " << structVar.getCleanedName()); 
        //   logout("original is " << sourceVar.getCleanedName());
        // }


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

void DataflowPass::insertAnnotation(Annotation* anno, ProgramVariable* pv) {
  AnnotationType annoType = anno->getAnnotationType();
  std::set<std::string> annoMethods =
      anno->getAnnotationMethods();
  MethodsSet *argMethods =
      pv->getMethodsSetRef();

  for (std::string annoMethod : annoMethods) {
    this->onAnnotation(argMethods, annoMethod, annoType);
  }
}

void DataflowPass::handleSretCall(CallInst* call, const std::string& fnName, const std::string& argName, ProgramPoint& programPoint) {
   if (call->getArgOperand(0)->getType()->isPointerTy()) {
    Type *pointerType = call->getArgOperand(0)->getType()->getPointerElementType();
    if (pointerType->isStructTy()) {
      StructType *structType = cast<StructType>(pointerType);
    
      int numFields = structType->getNumElements();

      for (int fieldIndex = 0; fieldIndex < numFields; fieldIndex++) {
        std::string fieldArg = argName + "." + std::to_string(fieldIndex);
        ProgramVariable* pvField = programPoint.getPVRef(fieldArg, false); 

        logout("found field " << pvField->getCleanedName());

        if (ReturnAnnotation *returnAnno = dynamic_cast<ReturnAnnotation *>(this->annotations.getReturnAnnotation(fnName, fieldIndex))) {
          logout("found annotation from sret " << returnAnno->generateStringRep()); 
          this->insertAnnotation(returnAnno, pvField);
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
          ProgramVariable* pvField = programPoint.getPVRef(fieldArg, false); 

          logout("found next arg fields " << pvField->getCleanedName() << " for j = " << j);

          auto allAnnotationsWithFields = this->annotations.getAllParameterAnnotationsWithFields(fnName); 
          for (Annotation* anno : allAnnotationsWithFields) {
            if (ParameterAnnotation *paramAnno = dynamic_cast<ParameterAnnotation *>(anno)) {
              if (paramAnno->getField() == fieldIndex) {
                logout("found param annotation " << paramAnno->generateStringRep());
                this->insertAnnotation(paramAnno, pvField);
              }
            }
          }                
        }
      } else {
        // arg is not a struct type
        auto allAnnotationsWithoutFields = this->annotations.getAllParameterAnnotationsWithoutFields(fnName); 
        for (Annotation* anno : allAnnotationsWithoutFields) {
          if (ParameterAnnotation *paramAnno = dynamic_cast<ParameterAnnotation *>(anno)) {
            // j - 1 because the 0th argument of the IR function call is not a part of
            // the actual function. the rest of the arguments are the ones a part of the call. 

            if (paramAnno->getNthParameter() == j - 1) {
              ProgramVariable* argVar = programPoint.getPVRef(nextArg, false); 
              logout("found j param annotation " << paramAnno->generateStringRep() << " for j = " << j << " and var " << argVar->getCleanedName());
              this->insertAnnotation(paramAnno, argVar);
            }
          }
        }      
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