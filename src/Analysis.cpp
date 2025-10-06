#include "Annotations/Annotation.h"
#include "Annotations/AnnotationHandler.h"
#include "Annotations/ErrorAnnotation.h"
#include "Annotations/FunctionAnnotation.h"
#include "Annotations/ParameterAnnotation.h"
#include "Annotations/ReturnAnnotation.h"
#include "Annotations/StructAnnotation.h"
#include "UtilFunctionTesters/UtilFunctionTester.h"
#include "UtilFunctionTesters/RLCDataflow/VariableTester.h"
#include "UtilFunctionTesters/RLCDataflow/UnwrapValuePointerToStructTester.h"
#include "UtilFunctionTesters/RLCDataflow/IRstructNameEqualsCstructNameTester.h"
#include "UtilFunctionTesters/RLCDataflow/GetPredecessorsTester.h"
#include "UtilFunctionTesters/RLCDataflow/GetSuccessorsTester.h"
#include "UtilFunctionTesters/RLCUtil/IsNumberTester.h"
#include "UtilFunctionTesters/RLCUtil/SplitStringTester.h"
#include "UtilFunctionTesters/RLCUtil/RemoveWhitespaceTester.h"
#include "UtilFunctionTesters/RLCUtil/SliceStringTester.h"
#include "UtilFunctionTesters/RLCUtil/StartsWithTester.h"
#include "UtilFunctionTesters/RLCUtil/SetToStringTester.h"
#include "CFG.h"
#include "CalledMethods.h"
#include "Constants.h"
#include "DataflowPass.h"
#include "Debug.h"
#include "MustCall.h"
#include "StructFieldToIndexMap.h"
#include "ProgramRepresentation/FullFile.h"
#include "StructFieldToIndexMap.h"
#include "LineNumberToLValueMap.h"
#include "LineNumberToLValueTester.h"
#include "Debug/BranchLister/ProgramLinesBranchInfo.h"
#include "RunAnalysis.h"
#include "TestRunner.h"
#include "BranchListerTester.h"
#include "StructFieldToIndexTester.h"
#include "FunctionInfoTester.h"
#include "TempFileManager.h"
#include "FunctionInfosManager.h"
#include "Utils.h"

// TODO: better test names in diff pr
// TODO: write testing for intentional errors (ErrorTestRunner)
// TODO: add error pairs to annotation test
// TODO: see for ast info generator we can filter out functions included from std lib
// TODO: make test cases for ASTAnalyses and have them ignore functions in stdlib
// TODO: add testing to ast_info_tokens/generator/reader to ensure that tweaking format doesn't change pass results
// TODO: getAnnotationStrings, FunctionInfosManager, StructFieldToIndexMap share code. consolidate into class & subclasses (ASTReaders)
// TODO: document FunctionInfo and FunctionInfosManager and get_function_info.py
// TODO: write testing for getTestName, getNthLine, getLLVMStructType, getFunctionArgs

struct InstructionHolder {
    SetVector<Instruction *> branch;
    SetVector<Instruction *> successors;
};

namespace rlc_dataflow {

std::set<std::string> SafeFunctions;
std::set<std::string> ReallocFunctions;
std::map<std::string, std::string> MemoryFunctions;
std::vector<std::string> realBranchOrder;
bool loadAndBuild = false;
bool anyTestFailed = false;
CalledMethods calledMethods;
MustCall mustCall;
AnnotationHandler annotationHandler;
StructFieldToIndexMap structFieldToIndexMap;
LineNumberToLValueMap lineNumberToLValueMap;
FunctionInfosManager functionInfosManager;
ProgramLinesBranchInfo programLinesBranchesInfo;
std::string cFileName;

void loadFunctions() {
    // working directory is /build

    std::ifstream safeFunctionsFile("../Functions/safe.txt");
    std::ifstream reallocFunctionsFile("../Functions/realloc.txt");
    std::ifstream memoryFunctionsFile("../Functions/memory.txt");

    std::string line;
    if (safeFunctionsFile.is_open()) {
        while (std::getline(safeFunctionsFile, line)) {
            SafeFunctions.insert(line);
        }
    }

    if (reallocFunctionsFile.is_open()) {
        while (std::getline(reallocFunctionsFile, line)) {
            ReallocFunctions.insert(line);
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
    memoryFunctionsFile.close();
    reallocFunctionsFile.close();
}

void buildCFG(CFG &topCFG, std::vector<std::string> branchOrder,
              std::map<std::string, InstructionHolder> branchInstMap) {
    topCFG = CFG(FIRST_BRANCH_NAME);
    std::map<std::string, CFG *> cfgMap;
    cfgMap[FIRST_BRANCH_NAME] = &topCFG;

    for (auto branchName : branchOrder) {
        auto succs = branchInstMap[branchName].successors;

        CFG *cfg = cfgMap[branchName];

        cfg->setInstructions(branchInstMap[branchName].branch);

        for (auto succ : succs) {
            std::string succName = succ->getParent()->getName().str();

            if (succName == branchName) {
                continue;
            }

            if (cfgMap.count(succName)) {
                cfg->addSuccessor(cfgMap[succName]);
                continue;
            }

            cfgMap[succName] = cfg->addSuccessor(succName);
        }
    }
}

std::vector<std::string> getAnnotationStrings(const TempFileManager& astInfoFile) {
    TempFileManager annotationsTempFile = TempFileManager("annotationsTempFile");

    std::string readASTCommand =
        "python3 " + AST_ANNO_PASS_LOCATION + " " +
        astInfoFile.getFileName() + " " + annotationsTempFile.getFileName();

    system(readASTCommand.c_str());

    logout("get anno strings to py run " << readASTCommand);

    std::ifstream annotationFile = annotationsTempFile.getFileStream();
    std::vector<std::string> annotations;

    std::string line;
    if (annotationFile.is_open()) {
        while (std::getline(annotationFile, line)) {
            logout("got anno: " << line);
            annotations.push_back(line);
        }
    }

    return annotations;
}

void doAliasReasoning(Instruction *instruction,
                      ProgramFunction &programFunction,
                      std::string optLoadFileName,
                      StructFieldToIndexMap structFieldToIndexMap,
                      FunctionInfosManager functionInfosManager,
                      LineNumberToLValueMap lineNumberToLValueMap) {
    bool includes = false;
    std::string branchName = instruction->getParent()->getName().str();
    for (auto branch : realBranchOrder) {
        if (branch == branchName) {
            includes = true;
            break;
        }
    }

    ProgramPoint *programPoint =
        programFunction.getProgramPointRef(branchName, true);

    if (!includes) {
        realBranchOrder.push_back(branchName);
    }

    if (LoadInst *load = dyn_cast<LoadInst>(instruction)) {
        logout("(load) name is " << variable(load) << " for "
               << variable(load->getPointerOperand()));
        std::string varName = variable(load->getPointerOperand());

        ProgramVariable receivingVar = ProgramVariable(load);
        ProgramVariable givingVar = ProgramVariable(load->getPointerOperand());

        logout("add alias for analysis loadinst");
        programPoint->addAlias(receivingVar, givingVar);

    } else if (StoreInst *store = dyn_cast<StoreInst>(instruction)) {
        logout("store inst " << *instruction);

        Value *valueToStore = store->getOperand(0);
        Value *receivingValue = store->getOperand(1);

        ProgramVariable varToStore = ProgramVariable(store->getOperand(0));

        // necessary check since the value to store could just be a number, and we
        // don't need to alias vars with nums. Also can cause name clashes; e.g.,
        // if we have some "ret i32 0", we'll be adding program variable 0, but
        // there may be in the IR something like "%0 = ...", and our code will
        // interpret these as aliased

        if (!varToStore.isIdentifier()) {
            return;
        }

        ProgramVariable receivingVar = ProgramVariable(store->getOperand(1));

        if (CallInst *call = dyn_cast<CallInst>(valueToStore)) {
            ProgramVariable callVar = ProgramVariable(call);
            logout("add alias for analysis storeinst call inst");

            // check for pointer reassignment; if so, the resource becomes un-aliased
            if (auto pvasRef = programPoint->getPVASRef(receivingVar, false)) {
                if (pvasRef->containsCallInstVar()) {
                    logout("pointer reassignment inst " << *instruction);
                    logout("receiving var " << receivingVar.getRawName());

                    logout(callVar.getRawName()
                           << " alias to "
                           << pvasRef->toString(false, true));


                    const DebugLoc &debugLoc = call->getDebugLoc();

                    if (lineNumberToLValueMap.lineNumberIsInMap(debugLoc.getLine())) {
                        std::string leftHandSide = lineNumberToLValueMap.get(debugLoc.getLine());

                        std::string potentialStructName = structFieldToIndexMap.get(leftHandSide);
                        if (potentialStructName != "") {
                            leftHandSide = potentialStructName;
                        }

                        logout("LHS = " << leftHandSide);
                        PVAliasSet* LHSpvas = programPoint->getPVASRef(leftHandSide, false);

                        if (LHSpvas) {
                            programPoint->unalias(pvasRef, leftHandSide, call, receivingVar);
                            return;
                        }
                    }
                }
            }

            programPoint->addAlias(callVar, receivingVar);
            return;
        }

        // check if two structs are being aliased. the structs must refer
        // to the same type. if they do not, they are not aliased;
        // it is safe to do this because worst case scenario,
        // it yields a false positive.
        if (valueToStore->getType()->isPointerTy() &&
                receivingValue->getType()->isPointerTy()) {
            StructType *valueStruct =
                rlc_dataflow::unwrapValuePointerToStruct(valueToStore);
            StructType *receivingStruct =
                rlc_dataflow::unwrapValuePointerToStruct(receivingValue);

            if (valueStruct && receivingStruct && valueStruct == receivingStruct) {
                logout("two structs to alias " << *store);
                int numFields = valueStruct->getNumElements();

                logout("pre alias");
                ProgramPoint::logoutProgramPoint(*programPoint, true);

                for (int i = 0; i < numFields; i++) {
                    ProgramVariable valueStructVar = ProgramVariable(valueToStore, i);
                    ProgramVariable receivingStructVar =
                        ProgramVariable(receivingValue, i);

                    programPoint->makeAliased(valueStructVar, receivingStructVar);
                }

                logout("post alias");
                ProgramPoint::logoutProgramPoint(*programPoint, true);

                return;
            }
        }

        logout("add alias for analysis storeinst else case");
        ProgramPoint::logoutProgramPoint(*programPoint, true);
        programPoint->addAlias(varToStore, receivingVar);
        ProgramPoint::logoutProgramPoint(*programPoint, true);

    } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(instruction)) {
        ProgramVariable sourceVar = ProgramVariable(bitcast);
        ProgramVariable destinationVar = ProgramVariable(bitcast->getOperand(0));

        StructType *sourceType = rlc_dataflow::unwrapValuePointerToStruct(bitcast);
        StructType *destType =
            rlc_dataflow::unwrapValuePointerToStruct(bitcast->getOperand(0));

        if (sourceType && destType && sourceType->hasName() &&
                destType->hasName()) {
            if (sourceType->getName() != destType->getName()) {
                errs() << "WARNING: Struct type conversions/bitcasting ('"
                       << destType->getName() << "' to '" << sourceType->getName()
                       << "') not supported. Related variables will not be considered "
                       "aliased, potentially causing false positives.\n";

                programPoint->addVariable(bitcast);

                int numFields = sourceType->getNumElements();
                for (int i = 0; i < numFields; i++) {
                    ProgramVariable sourceVar = ProgramVariable(bitcast, i);
                    programPoint->addVariable(sourceVar);
                }

                return;
            }
        }

        logout("add alias for analysis bitcast");
        programPoint->addAlias(sourceVar, destinationVar);

    } else if (GetElementPtrInst *gepInst =
                   dyn_cast<GetElementPtrInst>(instruction)) {

        // gepInsts typically take a struct and breaks it down into
        // its fields. an individual gepInst may represent one field of a struct.
        // note:
        /*
        LLVM removes field names and just makes them indices
        example:
        struct s {
          int a; // index 0
          int b; // index 1
          int c; // index 2
        }
        see:
        https://mapping-high-level-constructs-to-llvm-ir.readthedocs.io/en/latest/basic-constructs/structures.html
        */

        llvm::Type *structType = gepInst->getPointerOperandType();
        llvm::Value *pointerOperand = gepInst->getPointerOperand();


        if (llvm::PointerType *pointerType =
                    llvm::dyn_cast<llvm::PointerType>(pointerOperand->getType())) {
            if (llvm::StructType *structType =
                        llvm::dyn_cast<llvm::StructType>(pointerType->getElementType())) {
                llvm::Value *indexValue = gepInst->getOperand(2);
                if (llvm::ConstantInt *constIndex =
                            llvm::dyn_cast<llvm::ConstantInt>(indexValue)) {
                    ProgramVariable sourceVar = ProgramVariable(gepInst);
                    int index = constIndex->getValue().getSExtValue();

                    if (BitCastInst *bitcast = dyn_cast<BitCastInst>(pointerOperand)) {
                        ProgramVariable structVar =
                            ProgramVariable(bitcast->getOperand(0), index);
                        logout("add alias for analysis gepinst");
                        programPoint->addAlias(sourceVar, structVar);
                        return;
                    }

                    ProgramVariable structPV = ProgramVariable(pointerOperand);

                    PVAliasSet *originalStructPVASRef =
                        programPoint->getPVASRef(structPV, false);

                    if (!originalStructPVASRef) {
                        originalStructPVASRef = programFunction.getPVASRefFromValue(pointerOperand);

                        if (!originalStructPVASRef) {
                            errs() << "pvas struct ref not found by value " << *pointerOperand << ". early exit\n";
                            std::exit(1);
                        }
                    }


                    ProgramFunction::logoutProgramFunction(programFunction, false);

                    for (ProgramVariable pv :
                            originalStructPVASRef->getProgramVariables()) {
                        if (AllocaInst *structAllocaInst =
                                    dyn_cast<AllocaInst>(pv.getValue())) {
                            ProgramVariable structVar = ProgramVariable(pv.getValue(), index);

                            logout("spec index inst = " << *gepInst);
                            logout("specifying index for " << structVar.getCleanedName());

                            programPoint->addAlias(sourceVar, structVar);

                            return;
                        }
                    }
                }
            }
        }
    } else if (AllocaInst *allocate = dyn_cast<AllocaInst>(instruction)) {
        logout("alloca inst = " << *allocate);

        StructType *structType = rlc_dataflow::unwrapValuePointerToStruct(allocate);

        if (!structType) {
            return;
        }

        programPoint->addVariable(ProgramVariable(allocate));

        std::string structName = structType->getName().str();

        structName = rlc_util::sliceString(
                         structName, structName.find_last_of('.') + 1, structName.size() - 1);
        logout("struct name in IR = " << structName);

        if (!rlc_dataflow::IRstructNameEqualsCstructName(structName,
                optLoadFileName)) {
            errs() << "Error: Did not find struct name '" << structName
                   << "' in debug info\n";
            exit(1);
        }

        int numFields = structType->getNumElements();
        for (int i = 0; i < numFields; i++) {
            ProgramVariable sourceVar = ProgramVariable(allocate, i);
            programPoint->addVariable(sourceVar);
        }

    } else if (CallInst *call = dyn_cast<CallInst>(instruction)) {
        std::string fnName = call->getCalledFunction()->getName().str();
        /*
        there are 2 llvm annotations to consider:
        - llvm.ptr.annotation.*
         - https://llvm.org/docs/LangRef.html#llvm-ptr-annotation-intrinsic
         - the * "specifies an address space for the pointer"
         - "the first argument is a pointer to an integer value of arbitrary
        bitwidth (result of some expression), the second is a pointer to a global
        string, the third is a pointer to a global string which is the source file
        name, and the last argument is the line number."
        - llvm.var.annotation
         - https://llvm.org/docs/LangRef.html#llvm-var-annotation-intrinsic
         - "the first argument is a pointer to a value,
         the second is a pointer to a global string,
          the third is a pointer to a global string which is the source file name,
          and the last argument is the line number."


        there is also llvm.codeview.annotation
        (https://llvm.org/docs/LangRef.html#llvm-codeview-annotation-intrinsic)
        and llvm.annotation.*
        (https://llvm.org/docs/LangRef.html#llvm-annotation-intrinsic)
        but we wont need to worry about them; they hold no aliasing information
        */
        if (rlc_util::startsWith(fnName, LLVM_PTR_ANNOTATION) ||
                rlc_util::startsWith(fnName, LLVM_VAR_ANNOTATION)) {
            ProgramVariable sourceVar = ProgramVariable(call);
            ProgramVariable destinationVar = ProgramVariable(call->getArgOperand(0));
            logout("add alias for analysis callinst llvm annotation");
            programPoint->addAlias(sourceVar, destinationVar);
            return;
        }

        // now we check for un-aliasing

        auto fi = functionInfosManager.getFunction(fnName);
        if (fi && fi->getNumberOfParameters() != call->getNumArgOperands()) {
            auto args = getFunctionArgs(optLoadFileName, call);

            for (unsigned i = 0; i < fi->getNumberOfParameters(); i++) {
                ProgramVariable argumentVar = ProgramVariable(call->getArgOperand(i));
                std::string arg = argumentVar.getCleanedName();
                PVAliasSet *pvas = programPoint->getPVASRef(argumentVar, false);

                int numFields = rlc_dataflow::getStructNumberOfFields(optLoadFileName, fi->getNthParamType(i));

                if (numFields != -1) {
                    for (unsigned j = 0; j < numFields; j++) {
                        std::string targetArg = args[i] + "." + std::to_string(j);

                        logout("target arg1 " << targetArg);

                        if (i + j >= call->getNumArgOperands()) {
                            continue;
                        }

                        argumentVar = ProgramVariable(call->getArgOperand(i + j));
                        logout("argument var " << argumentVar.getRawName());

                        PVAliasSet* targetArgPvas = programPoint->getPVASRef(targetArg, false);

                        if (targetArgPvas) {
                            logout("found target " << targetArg);
                            programPoint->unalias(targetArgPvas, targetArg, argumentVar);
                        }
                    }
                } else {
                    std::string targetArg = args[i];

                    std::string potentialStructAndFieldName = structFieldToIndexMap.get(targetArg);
                    if (potentialStructAndFieldName != "") {
                        targetArg = potentialStructAndFieldName;
                    }

                    logout("target arg2 " << targetArg);

                    PVAliasSet* targetArgPvas = programPoint->getPVASRef(targetArg, false);

                    if (targetArgPvas) {
                        programPoint->unalias(pvas, targetArg, argumentVar);
                    }
                }
            }
            return;
        }

        for (unsigned i = 0; i < call->getNumArgOperands(); ++i) {
            ProgramVariable argumentVar = ProgramVariable(call->getArgOperand(i));
            std::string arg = argumentVar.getCleanedName();
            PVAliasSet *pvas = programPoint->getPVASRef(argumentVar, false);

            if (pvas && pvas->containsCallInstVar()) {
                logout("at call " << *call << " param " << i << " pvas contains call inst var");

                if (SafeFunctions.count(fnName)) {
                    logout("safe function -> skip");
                    continue;
                }

                bool doContinue = false;
                for (auto pair : MemoryFunctions) {
                    if (fnName == pair.second) {
                        logout("mem freeing function -> skip");
                        doContinue = true;
                        break;
                    }
                }

                if (doContinue) {
                    continue;
                }

                logout("not skipped i = " << i);

                logout(pvas->toString(false, true));

                auto args = getFunctionArgs(optLoadFileName, call);

                if (i < arg.size()) {
                    std::string targetArg = args[i];

                    std::string potentialStructAndFieldName = structFieldToIndexMap.get(targetArg);
                    if (potentialStructAndFieldName != "") {
                        targetArg = potentialStructAndFieldName;
                    }

                    logout("target arg " << targetArg << " for i = " << i);
                    logout(argumentVar.getRawName());

                    PVAliasSet* targetArgPvas = programPoint->getPVASRef(targetArg, false);

                    if (targetArgPvas) {
                        programPoint->unalias(pvas, targetArg, argumentVar);
                    }
                } else {
                    logout("missed out on " << arg << " for i = " << i);
                }
            }
        }
    }
}

ResourceLeakFunctionCallAnalyzerResult ResourceLeakFunctionCallAnalyzer::doAnalysis(Function &F, std::string optLoadFileName) {
    std::string fnName = F.getName().str();

    std::string testName = rlc_util::getTestName(optLoadFileName);

    programLinesBranchesInfo.add(F);

    bool functionIsKnown = false;
    logout("opt load file name = " << optLoadFileName);
    logout("Analyzing Function with Name = " << fnName
           << " test_name = " << testName);

    if (!loadAndBuild) {
        cFileName = optLoadFileName;

        loadFunctions();

        TempFileManager astInfoTempFile("astInfoTempFile");
        tempfile_util::dumpASTInfoIntoTempFile(optLoadFileName, astInfoTempFile);

        TempFileManager annotationsTempFile = TempFileManager("annotationsTempFile");

        auto annotations = getAnnotationStrings(astInfoTempFile);


        calledMethods.setExpectedResult(
            TestRunner::buildExpectedResults(testName, calledMethods.passName));
        mustCall.setExpectedResult(
            TestRunner::buildExpectedResults(testName, mustCall.passName));
        annotationHandler.addAnnotations(annotations);

        structFieldToIndexMap.buildMap(astInfoTempFile);
        lineNumberToLValueMap.buildMap(astInfoTempFile);
        functionInfosManager.buildFunctionInfo(astInfoTempFile);

        loadAndBuild = true;
    }

    // check if code re-defines a pre-defined function
    if (SafeFunctions.count(fnName)) {
        errs() << "**ANALYSIS-WARNING**: Re-definition of safe function '" << fnName
               << "' identified. Function erased from safe functions.\n";
        SafeFunctions.erase(fnName);
    }

    if (ReallocFunctions.count(fnName)) {
        errs() << "**ANALYSIS-WARNING**: Re-definition of realloc function '"
               << fnName
               << "' identified. Function erased from realloc functions.\n";
        ReallocFunctions.erase(fnName);
    }

    for (auto allocDeallocPair : MemoryFunctions) {
        std::string allocationFunction = allocDeallocPair.first;
        std::string deallocationFunction = allocDeallocPair.second;

        if (fnName == allocationFunction) {
            errs() << "**ANALYSIS-WARNING**: Re-definition of allocation function '"
                   << fnName
                   << "' identified. Function's alloc and dealloc functions erased "
                   "from memory functions.\n";
            MemoryFunctions.erase(fnName);
        }

        if (fnName == deallocationFunction) {
            errs() << "**ANALYSIS-WARNING**: Re-definition of deallocation function '"
                   << fnName
                   << "' identified. Function's alloc and dealloc functions erased "
                   "from memory functions.\n";
            MemoryFunctions.erase(fnName);
        }
    }

    ProgramFunction programFunction(fnName);
    std::map<std::string, InstructionHolder> branchInstructionMap;

    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
        std::string branchName = I->getParent()->getName().str();
        doAliasReasoning(&(*I), programFunction, optLoadFileName,
                         structFieldToIndexMap, functionInfosManager,
                         lineNumberToLValueMap);

        auto succs = rlc_dataflow::getSuccessors(&(*I));
        branchInstructionMap[branchName].branch.insert(&(*I));
        for (auto succ : succs) {
            branchInstructionMap[branchName].successors.insert(succ);
        }
    }

    CFG *cfg = new CFG();
    buildCFG(*cfg, realBranchOrder, branchInstructionMap);

    calledMethods.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions,
                               annotationHandler);
    calledMethods.setCFG(cfg);
    calledMethods.setProgramFunction(programFunction);
    calledMethods.setFunctionInfosManager(functionInfosManager);
    calledMethods.setOptLoadFileName(optLoadFileName);

    mustCall.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions,
                          annotationHandler);
    mustCall.setCFG(cfg);
    mustCall.setProgramFunction(programFunction);
    mustCall.setFunctionInfosManager(functionInfosManager);
    mustCall.setOptLoadFileName(optLoadFileName);

    ProgramFunction *PostCalledMethods = calledMethods.generatePassResults();
    ProgramFunction *PostMustCalls = mustCall.generatePassResults();

    /*

    logout("\n\nPROGRAM FUNCTION for " << programFunction.getFunctionName());
    ProgramFunction::logoutProgramFunction(programFunction, false);

    logout("\n\nCALLED METHODS RESULT");
    ProgramFunction::logoutProgramFunction(PostCalledMethods, true);

    logout("\n\nMUST CALL RESULT");
    ProgramFunction::logoutProgramFunction(PostMustCalls, true);

    errs() << "\n\nRUNNING CALLED METHODS TESTS - "
           << " TEST NAME - " << testName << "\n\n";

    std::string lastBranchName = realBranchOrder.back();

    bool calledMethodsResult = TestRunner::runTests(
                                   fnName, lastBranchName, calledMethods.getExpectedResult(),
                                   PostCalledMethods, structFieldToIndexMap);

    errs() << "\n\nRUNNING MUST CALL TESTS "
           << " TEST NAME - " << testName << "\n\n";
    bool mustCallResult = TestRunner::runTests(
                              fnName, lastBranchName, mustCall.getExpectedResult(), PostMustCalls, structFieldToIndexMap);

    if (calledMethodsResult == EXIT_FAILURE || mustCallResult == EXIT_FAILURE) {
        anyTestFailed = true;
    }

    if (BranchListerTester::runTest(testName, fnName, programLinesBranchesInfo) == EXIT_FAILURE) {
        logout("**BRANCH LISTER TESTER FAILED");
        anyTestFailed = true;
    } else {
        logout("BRANCH LISTER TESTER PASSED");
    }

    if (StructFieldToIndexTester::runTest(testName, structFieldToIndexMap) == EXIT_FAILURE) {
        logout("**STRUCT FIELD TO INDEX TESTER FAILED");
        anyTestFailed = true;
    } else {
        logout("STRUCT FIELD TO INDEX TESTER PASSED");
    }

    if (FunctionInfoTester::runTest(testName, functionInfosManager) == EXIT_FAILURE) {
        logout("**FUNCTION INFO TESTER FAILED");
        anyTestFailed = true;
    } else {
        logout("FUNCTION INFO TESTER PASSED");
    }

    if (LineNumberToLValueTester::runTest(testName, lineNumberToLValueMap) == EXIT_FAILURE) {
        logout("**LINE NUMBER TO L-VALUE TESTER FAILED");
        anyTestFailed = true;
    } else {
        logout("LINE NUMBER TO L-VALUE TESTER PASSED");
    }
    */
    realBranchOrder.clear();

    PostCalledMethods->getProgramPoints();
    return {PostCalledMethods, PostMustCalls};
}

// utilFunctionTester is an extended class of UtilFunctionTester
// (e.g., an instance of VariableTester)
void runUtilFunctionTester(UtilFunctionTester* utilFunctionTester, const std::string& functionName) {
    if (dynamic_cast<UtilFunctionTester*>(utilFunctionTester) == nullptr) {
        errs() << "ERROR: utilFunctionTester must be extended type of UtilFunctionTester\n";
        std::exit(EXIT_FAILURE);
    }


    errs() << "RUNNING UTIL FUNCTION TEST: " << functionName << "\n";
    if (!utilFunctionTester->runTest()) {
        errs() << "UTIL FUNCTION TEST FAIL: " << functionName << "\n";
        std::exit(EXIT_FAILURE);
    }

    errs() << "UTIL FUNCTION TEST PASS: " << functionName << "\n\n";
}

void ResourceLeakFunctionCallAnalyzer::onEnd() {
    std::cout << "why am I here\n";
    if (BUILD_PROGRAM_LINES_BRANCH_INFO) {
        programLinesBranchesInfo.generate(cFileName, false);
    }

    // rlc_dataflow function testers
    if (RUN_UTIL_FUNCTION_TESTS) {
        VariableTester vt = VariableTester();
        runUtilFunctionTester(&vt, "rlc_dataflow::variable");

        UnwrapValuePointerToStructTester uvptst = UnwrapValuePointerToStructTester();
        runUtilFunctionTester(&uvptst, "rlc_dataflow::unwrapValuePointerToStruct");

        IRstructNameEqualsCstructNameTester irsnecsnt = IRstructNameEqualsCstructNameTester();
        runUtilFunctionTester(&irsnecsnt, "rlc_dataflow::IRstructNameEqualsCstructName");

        GetPredecessorsTester getPredsTester = GetPredecessorsTester();
        runUtilFunctionTester(&getPredsTester, "rlc_dataflow::getPredecessors");

        GetSuccessorsTester getSuccsTester = GetSuccessorsTester();
        runUtilFunctionTester(&getSuccsTester, "rlc_dataflow::getSuccessors");

        // rlc_util function testers
        IsNumberTester isNumTester = IsNumberTester();
        runUtilFunctionTester(&isNumTester, "rlc_util::isNumber");

        SplitStringTester splitStringTester = SplitStringTester();
        runUtilFunctionTester(&splitStringTester, "rlc_util::splitString");

        RemoveWhitespaceTester removeWhitespaceTester = RemoveWhitespaceTester();
        runUtilFunctionTester(&removeWhitespaceTester, "rlc_util::removeWhitespace");

        SliceStringTester sliceStringTester = SliceStringTester();
        runUtilFunctionTester(&sliceStringTester, "rlc_util::sliceString");

        StartsWithTester startsWithTester = StartsWithTester();
        runUtilFunctionTester(&startsWithTester, "rlc_util::startsWith");

        SetToStringTester setToStringTester = SetToStringTester();
        runUtilFunctionTester(&setToStringTester, "rlc_util::setToString");
    }

    if (anyTestFailed) {
        std::exit(EXIT_FAILURE);
    }

    std::exit(EXIT_SUCCESS);
}

} // namespace rlc_dataflow
