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


bool onLoadInst(LoadInst *load, ProgramPoint *programPoint) {
    logout("(load) name is " << variable(load) << " for "
           << variable(load->getPointerOperand()));
    errs() << "INSTRUCTION IS " << *load << "\n";
    std::string varName = variable(load->getPointerOperand());

    ProgramVariable receivingVar = ProgramVariable(load);
    ProgramVariable givingVar = ProgramVariable(load->getPointerOperand());

    logout("add alias for analysis loadinst");
    return programPoint->addAlias(receivingVar, givingVar);
}

// this is called as a part of the store Instruction case, otherwise no aliases are created by calls (unless we add annotations for it later?)
// This represents moving a call result into an alias set, call results will always be put into registers so they will move everything from the alias they store
// into into the same alias as the call result
bool onCallInst(CallInst *call, ProgramVariable receivingVar,  ProgramPoint *programPoint, AAResults &AA) {
    ProgramVariable callVar = ProgramVariable(call);
    bool ret = false;
    logout("add alias for analysis storeinst call inst");
    logout("variable Name " << callVar.getCleanedName());
    logout("recv Name " << receivingVar.getCleanedName());

    // check for pointer reassignment; if so, the resource becomes un-aliased
    if (auto pvasRef = programPoint->getPVASRef(receivingVar, false)) {
        if (pvasRef->containsCallInstVar()) {
            logout("pointer reassignment inst " << *call);
            logout("receiving var " << receivingVar.getRawName());

            logout(callVar.getRawName()
                   << " alias to "
                   << pvasRef->toString(false, true));


            const DebugLoc &debugLoc = call->getDebugLoc();
            //
            //ERROR with ptr_struct_alloc is lefthandside failing

            if (lineNumberToLValueMap.lineNumberIsInMap(debugLoc.getLine())) {
                std::string leftHandSide = lineNumberToLValueMap.get(debugLoc.getLine());

                std::string potentialStructName = structFieldToIndexMap.get(leftHandSide);
                if (potentialStructName != "") {
                    leftHandSide = potentialStructName;
                }

                //PVAliasSet* LHSpvas = programPoint->getPVASRef(leftHandSide, false);
                PVAliasSet* LHSpvas = programPoint->getPVASRef(callVar, false);

                if (LHSpvas) {
                    // TODO: Problem here with deciding which aliases to move

                    PVAliasSet *q = programPoint->getPVASRef(receivingVar, false);
                    if(q == LHSpvas) {
                        return ret;
                    }
                    // Here only the storing alias in receivingVar must be moved to callVar PVAS
                    if(q) {
                        for(auto alias : programPoint->getPVASRef(receivingVar, false)->getProgramVariables()) {
                            if(AA.alias(receivingVar.getRealValue(), 8, alias.getRealValue(), 8) == AliasResult::MustAlias) {
                                std::cout << alias.getRawName() << " eq " << receivingVar.getRawName() << "\n";
                                ret = programPoint->addAlias(call, q->moveOut(alias)) || ret;
                            }
                        }
                    } else {
                        ret = programPoint->addAlias(call, receivingVar) || ret;
                    }
                }
            }
        }
    } else if(callVar.getCleanedName() != receivingVar.getCleanedName()) {
        ret = ret || programPoint->addAlias(receivingVar, callVar);
    } else if(!programPoint->getPVASRef(callVar, false)) {
        ret = ret || programPoint->addVariable(callVar);
    }

    return ret;
}

bool onCallNotStoreInst(CallInst *call, ProgramPoint *programPoint, std::string optLoadFileName) {
    std::string fnName = call->getCalledFunction()->getName().str();
    if (rlc_util::startsWith(fnName, LLVM_PTR_ANNOTATION) ||
            rlc_util::startsWith(fnName, LLVM_VAR_ANNOTATION) ||
            rlc_util::startsWith(fnName, LLVM_DBG_VALUE)) {
        //llvm var annotations should not be included
        return false;
    }
    bool change = false;

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
                /* This code unaliases struct fields that are passed as arguments.
                 * If a struct field is passed owning, obligations will be cleared so there will not be a concern of aliases
                 * if it is not passed owning, there is no change to alias information
                 * therefore nothing should be done here
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
                            change = programPoint->unalias(targetArgPvas, targetArg, argumentVar) || change;
                        }
                    }
                */
            } else {
                std::string targetArg = args[i];

                std::string potentialStructAndFieldName = structFieldToIndexMap.get(targetArg);
                if (potentialStructAndFieldName != "") {
                    targetArg = potentialStructAndFieldName;
                }

                logout("target arg2 " << targetArg);

                PVAliasSet* targetArgPvas = programPoint->getPVASRef(targetArg, false);

                if (targetArgPvas) {
                    change = programPoint->unalias(pvas, targetArg, argumentVar) || change;
                }
            }
        }
        return change;
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

            if (i < args.size()) {
                std::string targetArg = args[i];

                std::string potentialStructAndFieldName = structFieldToIndexMap.get(targetArg);
                if (potentialStructAndFieldName != "") {
                    targetArg = potentialStructAndFieldName;
                }

                logout("target arg " << targetArg << " for i = " << i);
                logout(argumentVar.getRawName());

                PVAliasSet* targetArgPvas = programPoint->getPVASRef(targetArg, false);

                if (targetArgPvas) {
                    change = programPoint->unalias(pvas, targetArg, argumentVar) || change;
                }
            } else {
                logout("missed out on " << arg << " for i = " << i);
            }
        }
    }

    return change;

}

bool onBitCastInst(BitCastInst *bitcast, ProgramPoint *programPoint) {
    bool ret = false;
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

            ret = ret || programPoint->addVariable(bitcast);

            int numFields = sourceType->getNumElements();
            for (int i = 0; i < numFields; i++) {
                //TODO: Check if this is actually needed
                ProgramVariable sourceVar = ProgramVariable(bitcast, i, bitcast);
                ret = ret || programPoint->addVariable(sourceVar);
            }

            return ret;
        }
    }

    logout("add alias for analysis bitcast");
    ret = ret || programPoint->addAlias(sourceVar, destinationVar);
    return ret;
}

bool onGetElementPtrInst(GetElementPtrInst *gepInst, ProgramPoint *programPoint, ProgramBlock *programBlock) {
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

    bool ret = false;
    llvm::Type *structType = gepInst->getPointerOperandType();
    llvm::Value *pointerOperand = gepInst->getPointerOperand();
    llvm::errs() << "INSTRUCTION IS " << *gepInst << "\n";

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
                        ProgramVariable(bitcast->getOperand(0), index, (Value *)gepInst);
                    logout("add alias for analysis gepinst");
                    errs() << sourceVar.getCleanedName() << " IS SOURCE AND " << structVar.getCleanedName() << " IS Struct\n";
                    ret = programPoint->addAlias(sourceVar, structVar) || ret;
                    return ret;
                }

                ProgramVariable structPV = ProgramVariable(pointerOperand);

                PVAliasSet *originalStructPVASRef =
                    programPoint->getPVASRef(structPV, false);
                if (!originalStructPVASRef) {
                    originalStructPVASRef = programBlock->getPVASRefFromValue(pointerOperand);

                    if (!originalStructPVASRef) {
                        errs() << "struct" << structPV.getCleanedName() << " not found, marking function as changed so that predecessors can process struct ref\n";
                        // (* Add some error here if no previous change has been committed to prevent infinite loop *)
                        return true;
                    }
                }

                for (ProgramVariable pv :
                        originalStructPVASRef->getProgramVariables()) {
                    if (AllocaInst *structAllocaInst =
                                dyn_cast<AllocaInst>(pv.getValue())) {
                        ProgramVariable structVar = ProgramVariable(pv.getValue(), index, (Value *)gepInst);

                        logout("spec index inst = " << *gepInst);
                        logout("specifying index for " << structVar.getCleanedName());

                        ret = programPoint->addAlias(sourceVar, structVar) || ret;

                        return ret;
                    }
                }
            }
        }
    }
    return ret;

}

bool onAllocaInst(AllocaInst *allocate, ProgramPoint *programPoint, std::string optLoadFileName) {
    bool ret = false;
    logout("alloca inst = " << *allocate);

    StructType *structType = rlc_dataflow::unwrapValuePointerToStruct(allocate);

    if (!structType) {
        return ret;
    }

    ret = ret || programPoint->addVariable(ProgramVariable(allocate));

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
        //TODO: Inspect if this is actually needed
        ProgramVariable sourceVar = ProgramVariable(allocate, i, allocate);
        ret = ret || programPoint->addVariable(sourceVar);
    }
    return ret;
}

// We convert the LLVM IR to SSA before this point
// SSA guarantees that because there is no information of a variable in a previous branch,
// it could not have been assigned as there is a uniqueness guarantee
//
// This lub works as follows, if the current program point lacks an alias in the old program point, it must
// be added. This is either done to instantiate the program point or done as a result of a merging of program blocks
// with sets of alias sets not present in each other
// In the first case, the copying of alias sets is equivalent to variables surviving after block execution
// In the second case, because the IR is converted into SSA, any aliases not present in one block implies that
// the resources associated with it are not allocated in the other program block
bool lubAlias(ProgramPoint *current, ProgramPoint *old) {
    std::list<PVAliasSet> currSets = current->getProgramVariableAliasSets().getSets();
    std::list<PVAliasSet> oldSets = old->getProgramVariableAliasSets().getSets();
    auto oldItFin = oldSets.begin(); // This is used because the following for loop will stop early when currSets stops
    bool ret = false;
    for(auto currit = currSets.begin(), oldit = oldSets.begin(); currit != currSets.end() && oldit != oldSets.end();) {
        // increment left intentionally blank as this changes based on what happens

        // this possibly represents a new set not present in the old set, this is maintained
        if(currit->getID() < oldit->getID()) {
            ++currit;
            continue;
        }
        // this represents needing to add an alias to current sets
        else if(currit->getID() > oldit->getID()) {
            ret = current->addPVAS(*oldit) || ret;
            ++oldit;
        }
        //Otherwise they are referring to the same set, so they must agree on aliases. Members found in currit but not oldit are removed
        else {
            for(auto currAlias : currit->getProgramVariables()) {
                if(!oldit->contains(currAlias)) {
                    currit->moveOut(currAlias);
                }
            }
            ++currit;
            ++oldit;
        }
        ++oldItFin;
    }

    while(oldItFin != oldSets.end()) {
        ret = current->addPVAS(*oldItFin) || ret;
        ++oldItFin;
    }

    return ret;
}

bool doAliasReasoning(Instruction *instruction,
                      ProgramFunction *programFunction,
                      std::string optLoadFileName,
                      StructFieldToIndexMap structFieldToIndexMap,
                      FunctionInfosManager functionInfosManager,
                      LineNumberToLValueMap lineNumberToLValueMap,
                      AAResults &AA) {
    // NEED LUB OF ALL PREDECESSOR's Last point here

    bool includes = false;
    bool change = false;

    BasicBlock *branch = instruction->getParent();

    std::string branchName = instruction->getParent()->getName().str();

    int instNum = 0;

    ProgramPoint *programPoint;
    ProgramBlock *currBlock = programFunction->getProgramBlockRef(branchName, true);
    if(instruction == &branch->front()) {
        programPoint = currBlock->getPoint(0, true);
        ProgramPoint::logoutProgramPoint(programPoint, true);
        programPoint->setParentFunc(programFunction);
        for(BasicBlock *succ : successors(branch)) {
            currBlock->addSuccessor(programFunction->getProgramBlockRef(succ->getName().str(), true));
        }
        ProgramPoint p = *programPoint;
        for(auto block : programFunction->getProgramBlocks()) {
            auto follows = block.getSuccessors();
            auto iter = std::find_if(follows.begin(), follows.end(),
            [branchName](const ProgramBlock *b) {
                return b->getBlockName() == branchName;
            });
            if(iter != follows.end()) {
                change = lubAlias(programPoint, block.getLast()) || change;
            }
        }
    } else {
        auto instIT = branch->begin();
        while(instruction != &*instIT) {
            ++instIT;
            ++instNum;
        }
    }
    programPoint = currBlock->getPoint(instNum, true);

    llvm::errs() << "BRANCH " << branchName << "\n";
    for (auto branch : realBranchOrder) {
        if (branch == branchName) {
            includes = true;
            break;
        }
    }

    llvm::errs() << "old is \n";
    ProgramPoint::logoutProgramPoint(programPoint, true);

    if (!includes) {
        realBranchOrder.push_back(branchName);
    }
    // a store instruction is how llvm IR handles mutable field reassignment, even in SSA
    if(StoreInst *store = dyn_cast<StoreInst>(instruction)) {
        llvm::errs() << "INSTRUCTION IS " << *store << "\n";
        Value *valueToStore = store->getOperand(0);
        Value *receivingValue = store->getOperand(1);
        ProgramVariable varToStore = ProgramVariable(store->getOperand(0));
        if (!varToStore.isIdentifier()) {
            PVAliasSet *pv = programPoint->getPVASRef(receivingValue, false);
            if(pv) {
                pv->moveOut(ProgramVariable(receivingValue));
            }
            return change;
        }
        ProgramVariable receivingVar = ProgramVariable(store->getOperand(1));

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
                    ProgramVariable valueStructVar = ProgramVariable(valueToStore, i, valueToStore);
                    //TODO: Check here for what to actually put into real for these
                    ProgramVariable receivingStructVar =
                        ProgramVariable(receivingValue, i, receivingValue);
                    change = programPoint->makeAliased(valueStructVar, receivingStructVar) || change;
                }

                logout("post alias");
                ProgramPoint::logoutProgramPoint(*programPoint, true);

                return change;
            }
        }

        if (CallInst *call = dyn_cast<CallInst>(valueToStore)) {
            ProgramVariable callVar = ProgramVariable(call);
            logout("add alias for analysis storeinst call inst SPEC");
            change = onCallInst(call, receivingVar, programPoint, AA) || change;
            return change;
        }

        logout("add alias for analysis storeinst else case");
        change = programPoint->addAlias(varToStore, receivingVar) || change;
    } else if (LoadInst *load = dyn_cast<LoadInst>(instruction)) {
        change = onLoadInst(load, programPoint) || change;
    } else if (CallInst *call = dyn_cast<CallInst>(instruction)) {
        if(!call->getType()->isVoidTy()) {
            change = onCallInst(call, call, programPoint, AA) || change;
        } else {
            change = onCallNotStoreInst(call, programPoint, optLoadFileName) || change;
        }
    } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(instruction)) {
        change = onBitCastInst(bitcast, programPoint) || change;
    } else if (AllocaInst *allocate = dyn_cast<AllocaInst>(instruction)) {
        change = onAllocaInst(allocate, programPoint, optLoadFileName) || change;
    } else if (GetElementPtrInst *gepInst =
                   dyn_cast<GetElementPtrInst>(instruction)) {
        change = onGetElementPtrInst(gepInst, programPoint, programFunction->getProgramBlockRef(branchName, false)) || change;
    }

    return change;
}

ResourceLeakFunctionCallAnalyzerResult ResourceLeakFunctionCallAnalyzer::doAnalysis(Function &F, std::string optLoadFileName, AAResults &AA) {
    std::string fnName = F.getName().str();

    std::string testName = rlc_util::getTestName(optLoadFileName);

    std::cout << fnName << "\n";
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

        /*
            calledMethods.setExpectedResult(
                TestRunner::buildExpectedResults(testName, calledMethods.passName));
            mustCall.setExpectedResult(
                TestRunner::buildExpectedResults(testName, mustCall.passName));
            */
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

    ProgramFunction *programFunction = new ProgramFunction(fnName);
    std::map<std::string, InstructionHolder> branchInstructionMap;
    ProgramBlock *entry = programFunction->getProgramBlockRef("entry", true);

    ProgramPoint *p = entry->getPoint(0, true);

    for(auto& Arg : F.args()) {
        Value *v = &Arg;
        llvm::errs() << "I AM " << *v << "\n";
        ProgramVariable create = ProgramVariable(v);
        p->addVariable(create);
        if(v->getType()->isPointerTy()) {
            std::string n =  create.getCleanedName() + ".addr" ;
            llvm::errs() << n << "\n";
            ProgramVariable createNew = create.copyNewName(n);
            p->addVariable(createNew);
            ProgramPoint::logoutProgramPoint(p, true);
            p->makeAliased(create, createNew);
        }
    }


    bool fixed = true;
    while(fixed) {
        llvm::errs() << "LOOPING\n";
        fixed = false;
        for(inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
            std::string branchName = I->getParent()->getName().str();
            logout(branchName);
            ProgramBlock b = *programFunction->getProgramBlockRef(branchName, true);
            fixed = doAliasReasoning(&(*I), programFunction, optLoadFileName,
                                     structFieldToIndexMap, functionInfosManager,
                                     lineNumberToLValueMap, AA) || fixed;
            llvm::errs() << "FIXED IS " << fixed << "\n";
        }
    }
    llvm::errs() << "\n\n\n" << "After Alias Analysis: \n\n\n";
    for(auto b : programFunction->getProgramBlocks()) {
        for(auto p : b.getPoints()) {
            ProgramPoint::logoutProgramPoint(p, true);
            logout("\n-----\n");
        }
    }

    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
        std::string branchName = I->getParent()->getName().str();
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
    calledMethods.setFunc(&F);
    calledMethods.setProgramFunction(programFunction->deepCopy());
    calledMethods.setFunctionInfosManager(functionInfosManager);
    calledMethods.setOptLoadFileName(optLoadFileName);

    mustCall.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions,
                          annotationHandler);
    mustCall.setCFG(cfg);
    mustCall.setFunc(&F);
    mustCall.setProgramFunction(programFunction->deepCopy());
    mustCall.setFunctionInfosManager(functionInfosManager);
    mustCall.setOptLoadFileName(optLoadFileName);

    ProgramFunction *PostCalledMethods = calledMethods.generatePassResults();
    ProgramFunction *PostMustCalls = mustCall.generatePassResults();

    for(auto b : PostCalledMethods->getProgramBlocks()) {
        for(auto p : b.getPoints()) {
            ProgramPoint::logoutProgramPoint(p, true);
        }
    }

    for(auto b : PostMustCalls->getProgramBlocks()) {
        for(auto p : b.getPoints()) {
            ProgramPoint::logoutProgramPoint(p, true);
        }
    }

    logout("\n\nPROGRAM FUNCTION for " << programFunction->getFunctionName());
    ProgramFunction::logoutProgramFunction(*programFunction, false);
    delete programFunction;

    logout("\n\nCALLED METHODS RESULT");
    ProgramFunction::logoutProgramFunction(*PostCalledMethods, true);

    logout("\n\nMUST CALL RESULT");
    ProgramFunction::logoutProgramFunction(*PostMustCalls, true);

    errs() << "\n\nRUNNING CALLED METHODS TESTS - "
           << " TEST NAME - " << testName << "\n\n";

    std::string lastBranchName = realBranchOrder.back();

    bool calledMethodsResult = TestRunner::runTests(
                                   fnName, lastBranchName, calledMethods.getExpectedResult(),
                                   *PostCalledMethods, structFieldToIndexMap);

    errs() << "\n\nRUNNING MUST CALL TESTS "
           << " TEST NAME - " << testName << "\n\n";
    bool mustCallResult = TestRunner::runTests(
                              fnName, lastBranchName, mustCall.getExpectedResult(), *PostMustCalls, structFieldToIndexMap);

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
    realBranchOrder.clear();
    PostMustCalls->setAnnotationHandler(annotationHandler);
    PostCalledMethods->setAnnotationHandler(annotationHandler);

    return {PostMustCalls, PostCalledMethods};
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

} // namespace rlc_dataflow
