#include "Annotations/Annotation.h"
#include "Annotations/AnnotationHandler.h"
#include "Annotations/ErrorAnnotation.h"
#include "Annotations/FunctionAnnotation.h"
#include "Annotations/ParameterAnnotation.h"
#include "Annotations/ReturnAnnotation.h"
#include "Annotations/StructAnnotation.h"
#include "CFG.h"
#include "CalledMethods.h"
#include "DataflowPass.h"
#include "Debug.h"
#include "MustCall.h"
#include "ProgramRepresentation/FullProgram.h"
#include "RunAnalysis.h"
#include "TestRunner.h"
#include "Utils.h"


// to run: cd build   then
// sh ../run_test.sh <test_num>
// or to run all tests: sh ../run_all.sh
// note for run all tests is that if you add more tests, you'll have to modify
// run_all.sh to include that test number

// if a known function is being re-defined, issue a warning and
// remove it from safe/realloc/memory functions (wherever it's in).
// it's annotations should be checked (once annotations are fully implemented)

struct InstructionHolder {
  SetVector<Instruction *> branch;
  SetVector<Instruction *> successors;
};

namespace dataflow {

std::set<std::string> SafeFunctions;
std::set<std::string> ReallocFunctions;
std::map<std::string, std::string> MemoryFunctions;
std::vector<std::string> realBranchOrder;
bool loadAndBuild = false;
bool anyTestFailed = false;
CalledMethods calledMethods;
MustCall mustCall;
AnnotationHandler annotationHandler;

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

std::string getTestName(std::string optLoadFileName) {
  int slashCount = 0;
  std::string testName;
  // assuming that format of optLoadFileName is ../test/test{num}.c, returns
  // test{num}
  for (char c : optLoadFileName) {
    if (c == '/') {
      slashCount++;
      continue;
    }

    if (slashCount == 2) {
      if (c == '.')
        break;

      testName += c;
    }
  }
  return testName;
}

void buildCFG(CFG &topCFG, std::vector<std::string> branchOrder,
              std::map<std::string, InstructionHolder> branchInstMap) {
  topCFG = CFG("entry");
  std::map<std::string, CFG *> cfgMap;
  cfgMap["entry"] = &topCFG;

  for (auto branchName : branchOrder) {
    auto succs = branchInstMap[branchName].successors;

    CFG *cfg = cfgMap[branchName];

    cfg->setInstructions(branchInstMap[branchName].branch);

    for (auto succ : succs) {
      std::string succName = succ->getParent()->getName().str();

      if (succName == branchName)
        continue;

      if (cfgMap.count(succName) > 0) {
        cfg->addSuccessor(cfgMap[succName]);
        continue;
      }

      cfgMap[succName] = cfg->addSuccessor(succName);
    }
  }
}

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

void doAliasReasoning(Instruction *instruction,
                      SetVector<Instruction *> &workSet,
                      ProgramFunction &programFunction) {
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

  if (LoadInst *Load = dyn_cast<LoadInst>(instruction)) {
    logout("(load) name is " << variable(Load) << " for "
                             << variable(Load->getPointerOperand()))
        std::string varName = variable(Load->getPointerOperand());

    ProgramVariable receivingVar = ProgramVariable(Load);
    ProgramVariable givingVar = ProgramVariable(Load->getPointerOperand());

    programPoint->addAlias(receivingVar, givingVar);

  } else if (StoreInst *store = dyn_cast<StoreInst>(instruction)) {
    Value *valueToStore = store->getOperand(0);
    Value *receivingValue = store->getOperand(1);

    ProgramVariable varToStore = ProgramVariable(store->getOperand(0));

    // necessary check since the value to store could just be a number, and we
    // don't need to alias vars with nums. Also can cause name clashings; e.g.,
    // if we have some "ret i32 0", we'll be adding program variable 0, but
    // there may be in the IR something like "%0 = ...", and our code will
    // interpret these as aliased

    if (!varToStore.isIdentifier()) {
      return;
    }

    ProgramVariable receivingVar = ProgramVariable(store->getOperand(1));

    for (auto Inst : workSet) {
      if (valueToStore == Inst) {
        if (CallInst *call = dyn_cast<CallInst>(Inst)) {
          ProgramVariable callVar = ProgramVariable(call);
          programPoint->addAlias(callVar, receivingVar);
          return;
        }
      }
    }

    programPoint->addAlias(varToStore, receivingVar);

  } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(instruction)) {
    ProgramVariable sourceVar = ProgramVariable(bitcast);
    ProgramVariable destinationVar = ProgramVariable(bitcast->getOperand(0));

    programPoint->addAlias(sourceVar, destinationVar);

  } else if (GetElementPtrInst *gepInst =
                 dyn_cast<GetElementPtrInst>(instruction)) {

    // for some struct k { int x; int y }, here are two example getInst:
    // %y = getelementptr inbounds %struct.my_struct, %struct.my_struct* %k, i32
    // 0, i32 1, !dbg !57 %x = getelementptr inbounds %struct.my_struct,
    // %struct.my_struct* %k, i32 0, i32 0, !dbg !54 last argument on RHS is the
    // index of the struct
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

    // gepinst could also look like:
    // %2 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %1, i32 0, i32 0,
    // !dbg !84 in which case we see if %1 is bitcast: %1 = bitcast
    // %struct.my_struct* %k to { i8*, i8* }*, !dbg !84 then we alias %k.0 and
    // %2

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
            programPoint->addAlias(sourceVar, structVar);
            return;
          }

          ProgramVariable structVar = ProgramVariable(pointerOperand, index);
          programPoint->addAlias(sourceVar, structVar);
        }
      }
    }
  } else if (AllocaInst *allocate = dyn_cast<AllocaInst>(instruction)) {
    if (llvm::StructType *structType =
            llvm::dyn_cast<llvm::StructType>(allocate->getAllocatedType())) {
      /*
      I cannot find documentation that states how struct names appear in the IR,
      but it seems that they typically look like "struct.[C struct name]"
      TODO: prove/disprove that struct names are formatted as "struct.[C struct
      name]"
      */
      std::string structName = structType->getName();
      structName = sliceString(structName, structName.find_last_of('.') + 1,
                               structName.size() - 1);
      int numFields = structType->getNumElements();
      for (int i = 0; i < numFields; i++) {

        Annotation *anno = annotationHandler.getStructAnnotation(
            structName, std::to_string(i));
        if (StructAnnotation *structAnno =
                dynamic_cast<StructAnnotation *>(anno)) {
          std::set<std::string> structMethods =
              structAnno->getAnnotationMethods();
          AnnotationType structAnnoType = structAnno->getAnnotationType();
          ProgramVariable sourceVar = ProgramVariable(allocate, i);
          programPoint->addVariable(sourceVar);
        }
      }
    }
  }
}

void CodeAnalyzer::doAnalysis(Function &F, std::string optLoadFileName) {
  logout("DO ANALYSIS NOT RUNNING")
  return; 

  SetVector<Instruction *> WorkSet;
  std::string fnName = F.getName().str();

  std::string testName = getTestName(optLoadFileName);

  bool functionIsKnown = false;
  logout("Analyzing Function with Name = " << fnName << " opt load file name = "
                                           << testName)

      if (!loadAndBuild) {
    loadFunctions();
    calledMethods.setExpectedResult(
        TestRunner::buildExpectedResults(testName, calledMethods.passName));
    mustCall.setExpectedResult(
        TestRunner::buildExpectedResults(testName, mustCall.passName));
    annotationHandler.addAnnotationsFromFile("../test/" + testName + ".ll");
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

  for (auto Pair : MemoryFunctions) {
    if (fnName == Pair.first) {
      errs() << "**ANALYSIS-WARNING**: Re-definition of allocation function '"
             << fnName
             << "' identified. Function's alloc and dealloc functions erased "
                "from memory functions.\n";
      MemoryFunctions.erase(fnName);
    }

    if (fnName == Pair.second) {
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
    WorkSet.insert(&(*I));

    std::string branchName = I->getParent()->getName().str();
    doAliasReasoning(&(*I), WorkSet, programFunction);

    auto succs = getSuccessors(&(*I));
    branchInstructionMap[branchName].branch.insert(&(*I));
    for (auto succ : succs) {
      branchInstructionMap[branchName].successors.insert(succ);
    }
  }

  CFG TopCFG;
  buildCFG(TopCFG, realBranchOrder, branchInstructionMap);

  calledMethods.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions,
                             annotationHandler);
  calledMethods.setCFG(&TopCFG);
  calledMethods.setProgramFunction(programFunction);

  mustCall.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions,
                        annotationHandler);
  mustCall.setCFG(&TopCFG);
  mustCall.setProgramFunction(programFunction);

  ProgramFunction PostCalledMethods = calledMethods.generatePassResults();
  ProgramFunction PostMustCalls = mustCall.generatePassResults();

  logout(
      "\n\nPROGRAM FUNCTION for "
      << programFunction.getName()) for (auto point :
                                         programFunction.getProgramPoints()) {
    logout("\n**point name " << point.getName());
    for (auto var : point.getProgramVariables()) {
      logout(">var name " << var.getRawName() << " | " << var.getCleanedName())
          std::set<std::string>
              methods = var.getMethodsSet().getMethods();
      logout("methods set " << dataflow::setToString(methods))

          auto aliases = var.getAllAliases(true);
      auto aliasesStr = dataflow::setToString(aliases);
      logout(">>aliases " << aliasesStr)
    }
  }

  logout(
      "\n\nCALLED METHODS RESULT") for (auto point :
                                        PostCalledMethods.getProgramPoints()) {
    logout("\n**point name " << point.getName());
    for (auto var : point.getProgramVariables()) {
      logout(">var name " << var.getRawName() << " | " << var.getCleanedName())
          std::set<std::string>
              methods = var.getMethodsSet().getMethods();
      logout("methods set " << dataflow::setToString(methods))

          auto aliases = var.getAllAliases(true);
      auto aliasesStr = dataflow::setToString(aliases);
      logout(">>aliases " << aliasesStr)
    }
  }

  logout("\n\nMUST CALL RESULT") for (auto point :
                                      PostMustCalls.getProgramPoints()) {
    logout("\n**point name " << point.getName());
    for (auto var : point.getProgramVariables()) {
      logout(">var name " << var.getRawName() << " | " << var.getCleanedName())
          std::set<std::string>
              methods = var.getMethodsSet().getMethods();
      logout("methods set " << dataflow::setToString(methods))

          auto aliases = var.getAllAliases(true);
      auto aliasesStr = dataflow::setToString(aliases);
      logout(">>aliases " << aliasesStr)
    }
  }

  errs() << "\n\nRUNNING CALLED METHODS TESTS - "
         << " TEST NAME - " << testName << "\n\n";

  std::string lastBranchName = realBranchOrder.back();

  bool calledMethodsResult = TestRunner::runTests(
      fnName, lastBranchName, calledMethods.getExpectedResult(),
      PostCalledMethods);

  errs() << "\n\nRUNNING MUST CALL TESTS "
         << " TEST NAME - " << testName << "\n\n";
  bool mustCallResult = TestRunner::runTests(
      fnName, lastBranchName, mustCall.getExpectedResult(), PostMustCalls);

  if (calledMethodsResult == EXIT_FAILURE || mustCallResult == EXIT_FAILURE) {
    anyTestFailed = true;
  }
}

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

class MyASTConsumer : public ASTConsumer {
public:
    MyASTConsumer() {}

    void HandleTranslationUnit(ASTContext &Context) override {
        // Define an ASTMatcher to match specific comments or annotations.
        StatementMatcher CommentMatcher = commentStmt().bind("annotation");

        // Create an ASTMatchFinder and add the CommentMatcher to it.
        MatchFinder Finder;
        Finder.addMatcher(CommentMatcher, this);

        // Perform the AST traversal and match comments.
        Finder.matchAST(Context);
    }

    void run(const MatchFinder::MatchResult &Result) override {
        if (const Stmt *S = Result.Nodes.getNodeAs<Stmt>("annotation")) {
            llvm::outs() << "Annotation: " << S->getStmtClassName() << "\n";
        }
    }
};

void CodeAnalyzer::buildAST(std::string optLoadFileName) {
  // std::string testName = getTestName(optLoadFileName);
  // TODO: update getTestName. on something like "../test/test23/test24.ll", it'll error 

    logout("file name = " << optLoadFileName)

  // make ; clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test23/test24.c -o ../test/test23/test24.ll ; opt -load CodeAnalyzer.so -CodeAnalyzer ../test/test23/test24.ll

  // clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/test23/test24.c > ../test/test23/test24_ast.txt

  // clang -Xclang -ast-print -fsyntax-only -fno-color-diagnostics ../test/test23/test24.c > ../test/test23/test24_ast.txt

   std::ifstream sourceFile(optLoadFileName);

   std::string sourceCode((std::istreambuf_iterator<char>(sourceFile)), std::istreambuf_iterator<char>());

    ClangTool Tool;
    Tool.buildASTs();
    Tool.run(newFrontendActionFactory<MyASTConsumer>().get(), optLoadFileName);


}

void CodeAnalyzer::onEnd() {
  if (anyTestFailed) {
    std::exit(EXIT_FAILURE);
  } else {
    std::exit(EXIT_SUCCESS);
  }
}

} // namespace dataflow