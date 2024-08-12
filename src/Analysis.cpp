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
#include "UtilFunctionTesters/RLCUtil/IsValidCVariableNameTester.h"
#include "UtilFunctionTesters/RLCUtil/HasOnlyOneBalancedParenthesesTester.h"
#include "UtilFunctionTesters/RLCUtil/StartsWithTester.h"
#include "CFG.h"
#include "CalledMethods.h"
#include "Constants.h"
#include "DataflowPass.h"
#include "Debug.h"
#include "MustCall.h"
#include "StructFieldToIndexMap.h"
#include "ProgramRepresentation/FullFile.h"
#include "StructFieldToIndexMap.h"
#include "Debug/BranchLister/ProgramLinesBranchInfo.h"
#include "RunAnalysis.h"
#include "TestRunner.h"
#include "TempFileManager.h"
#include "FunctionInfosManager.h"
#include "Utils.h"

// TODO: remove predecessors from CFG; unused
// TODO: handle un-aliasing
// TODO: better test names in diff pr
// TODO: write testing for intentional errors (ErrorTestRunner)
// TODO: add const to method params where it applies; specifically, specifying more const std::string& type
// TODO: add error pairs to annotation test
// TODO:  CLT that generates AST based on test name (for debugging). if file name given, pipe into file. if not, log to terminal
// TODO: see for ast info generator we can filter out functions included from std lib
// TODO: make test cases for ASTAnalyses and have them ignore functions in stdlib
// TODO: add testing to ast_info_tokens/generator/reader to ensure that tweaking format doesn't change pass results
// TODO: getAnnotationStrings, FunctionInfosManager, StructFieldToIndexMap share code. consolidate into class & subclasses (ASTReaders)
// TODO: write test code for FI pass
// TODO: implement FI in DataflowPass.cpp
// TODO: document FunctionInfo and FunctionInfosManager and get_function_info.py
// TODO: make sliceString params use unsigned instead of signed int

// IMPORTANT:
// TODO: when doing something like func(thing) make sure called methods is applied on thing when it gets desugared.
// update tests accordingly

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

// for some .c file ../test/<dir>/<file_name>.c, <dir>/<file_name> is returned.
// e.g., ../test/simple_layer_test/layer/test1_again.c ->
// simple_layer_test/layer/test1_again
std::string getTestName(std::string optLoadFileName) {
    std::string startsWith = "../test";
    std::string endsWith = ".c";
    optLoadFileName.replace(0, startsWith.length() + 1, "");
    optLoadFileName.erase(optLoadFileName.length() - 2);

    logout("RES = " << optLoadFileName);

    return optLoadFileName;
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
                      std::string optLoadFileName) {
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

        std::string structName = structType->getName();

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
        }
    }
}

void CodeAnalyzer::doAnalysis(Function &F, std::string optLoadFileName) {
    std::string fnName = F.getName().str();

    std::string testName = getTestName(optLoadFileName);

    if (BUILD_PROGRAM_LINES_BRANCH_INFO) {
        programLinesBranchesInfo.add(F);
    }

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
        doAliasReasoning(&(*I), programFunction, optLoadFileName);

        auto succs = rlc_dataflow::getSuccessors(&(*I));
        branchInstructionMap[branchName].branch.insert(&(*I));
        for (auto succ : succs) {
            branchInstructionMap[branchName].successors.insert(succ);
        }
    }

    CFG cfg;
    buildCFG(cfg, realBranchOrder, branchInstructionMap);

    calledMethods.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions,
                               annotationHandler);
    calledMethods.setCFG(&cfg);
    calledMethods.setProgramFunction(programFunction);
    calledMethods.setFunctionInfosManager(functionInfosManager);
    calledMethods.setOptLoadFileName(optLoadFileName);

    mustCall.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions,
                          annotationHandler);
    mustCall.setCFG(&cfg);
    mustCall.setProgramFunction(programFunction);
    mustCall.setFunctionInfosManager(functionInfosManager);
    mustCall.setOptLoadFileName(optLoadFileName);

    ProgramFunction PostCalledMethods = calledMethods.generatePassResults();
    ProgramFunction PostMustCalls = mustCall.generatePassResults();

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
}

// assumes utilFunctionTester is actually an extended class and not UtilFunctionTester
// (e.g., an instance of VariableTester)
void runUtilFunctionTester(UtilFunctionTester* utilFunctionTester, const std::string& functionName) {
    errs() << "RUNNING UTIL FUNCTION TEST: " << functionName << "\n";
    if (!utilFunctionTester->runTest()) {
        errs() << "UTIL FUNCTION TEST FAIL: " << functionName << "\n";
        std::exit(EXIT_FAILURE);
    }

    errs() << "UTIL FUNCTION TEST PASS: " << functionName << "\n\n";
}

void CodeAnalyzer::onEnd() {
    if (BUILD_PROGRAM_LINES_BRANCH_INFO) {
        programLinesBranchesInfo.generate(cFileName, true);
    }

    // rlc_dataflow function testers
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

    IsValidCVariableNameTester isValidCVarNameTestesr = IsValidCVariableNameTester();
    runUtilFunctionTester(&isValidCVarNameTestesr, "rlc_util::isValidCVariableName");

    HasOnlyOneBalancedParenthesesTester hasOnlyOneBalParenTeseter = HasOnlyOneBalancedParenthesesTester();
    runUtilFunctionTester(&hasOnlyOneBalParenTeseter, "rlc_util::hasOnlyOneBalancedParentheses");

    StartsWithTester startsWithTester = StartsWithTester();
    runUtilFunctionTester(&startsWithTester, "rlc_util::startsWith");

    if (anyTestFailed) {
        std::exit(EXIT_FAILURE);
    }

    std::exit(EXIT_SUCCESS);
}

} // namespace rlc_dataflow