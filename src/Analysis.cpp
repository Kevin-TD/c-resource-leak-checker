#include "Annotation.h"
#include "CFG.h"
#include "CalledMethods.h"
#include "DataflowPass.h"
#include "Debug.h"
#include "ErrorAnnotation.h"
#include "FunctionAnnotation.h"
#include "MustCall.h"
#include "ParameterAnnotation.h"
#include "ProgramVariablesHandler.h"
#include "ReturnAnnotation.h"
#include "RunAnalysis.h"
#include "StructAnnotation.h"
#include "TestRunner.h"
#include "Utils.h"

#include <fstream>
#include <map>

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
MappedMethods ExpectedResult;
bool loadAndBuild = false;
bool doAnnos = false;
CalledMethods calledMethods;
MustCall mustCall;

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
                      ProgramVariablesHandler &programVariables) {
  bool includes = false;
  std::string branchName = instruction->getParent()->getName().str();
  for (auto branch : realBranchOrder) {
    if (branch == branchName) {
      includes = true;
      break;
    }
  }
  if (!includes) {
    realBranchOrder.push_back(branchName);
  }

  if (LoadInst *Load = dyn_cast<LoadInst>(instruction)) {
    logout("(load) name is " << variable(Load) << " for "
                             << variable(Load->getPointerOperand()))
        std::string varName = variable(Load->getPointerOperand());

    ProgramVariable receivingVar = ProgramVariable(Load);
    ProgramVariable givingVar = ProgramVariable(Load->getPointerOperand());

    programVariables.addAlias(receivingVar, givingVar);

  } else if (StoreInst *store = dyn_cast<StoreInst>(instruction)) {
    Value *valueToStore = store->getOperand(0);
    Value *receivingValue = store->getOperand(1);

    ProgramVariable varToStore = ProgramVariable(store->getOperand(0));
    ProgramVariable receivingVar = ProgramVariable(store->getOperand(1));

    for (auto Inst : workSet) {
      if (valueToStore == Inst) {
        if (CallInst *call = dyn_cast<CallInst>(Inst)) {
          return;
        }
      }
    }

    programVariables.addAlias(varToStore, receivingVar);

  } else if (BitCastInst *bitcast = dyn_cast<BitCastInst>(instruction)) {
    std::string source = variable(bitcast);
    std::string destination = variable(bitcast->getOperand(0));

    ProgramVariable sourceVar = ProgramVariable(bitcast);
    ProgramVariable destinationVar = ProgramVariable(bitcast->getOperand(0));

    programVariables.addAlias(sourceVar, destinationVar);
  }
}

void CalledMethodsAnalysis::doAnalysis(Function &F,
                                       std::string optLoadFileName) {
  SetVector<Instruction *> WorkSet;
  std::string fnName = F.getName().str();

  std::string testName = getTestName(optLoadFileName);

  bool functionIsKnown = false;
  logout("fnname = " << fnName << " opt load file name = " << testName)

      if (!loadAndBuild) {
    loadFunctions();
    calledMethods.setExpectedResult(
        TestRunner::buildExpectedResults(testName, calledMethods.passName));
    mustCall.setExpectedResult(
        TestRunner::buildExpectedResults(testName, mustCall.passName));
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

  if (!doAnnos) {
    // annotation parsing is completed, next is the following TODO's

    // TODO: move annotation reasoning to separate class
    // TODO: change from using "doAnnos"; perhaps include in the loadAndBuild
    // once more handling for annotations is built and we can call something
    // like .generateAnnotations() annotation class should hold all the code's
    // annotations and then you'd be able to do something like
    // getAnnotation(struct name = ..., field = ...) and it would return the
    // annotion type (MustCall/CalledMethods) and methods (free)

    LLVMContext context;
    SMDiagnostic error;

    std::unique_ptr<Module> M =
        parseIRFile("../test/" + testName + ".ll", error, context);

    for (llvm::GlobalVariable &globalVar : M->globals()) {
      if (globalVar.hasInitializer()) {
        llvm::Constant *initializer = globalVar.getInitializer();
        if (llvm::ConstantDataSequential *dataSeq =
                llvm::dyn_cast<llvm::ConstantDataSequential>(initializer)) {
          if (dataSeq->isString()) {
            std::string stringValue = dataSeq->getAsString().str();
            llvm::outs() << "String: " << stringValue << "\n";

            Annotation *anno = generateAnnotation(stringValue);

            if (StructAnnotation *sa = dynamic_cast<StructAnnotation *>(anno)) {
              logout(sa->generateStringRep() << "\n")
            } else if (FunctionAnnotation *sa =
                           dynamic_cast<FunctionAnnotation *>(anno)) {
              logout(sa->generateStringRep() << "\n")
            } else if (ParameterAnnotation *sa =
                           dynamic_cast<ParameterAnnotation *>(anno)) {
              logout(sa->generateStringRep() << "\n")
            } else if (ReturnAnnotation *sa =
                           dynamic_cast<ReturnAnnotation *>(anno)) {
              logout(sa->generateStringRep() << "\n")
            } else if (ErrorAnnotation *sa =
                           dynamic_cast<ErrorAnnotation *>(anno)) {
              logout("undefined annotation\n")
            }
          }
        }
      }
    }

    doAnnos = true;
  }

  ProgramVariablesHandler AliasedProgramVars;
  std::map<std::string, InstructionHolder> branchInstructionMap;

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    WorkSet.insert(&(*I));

    std::string branchName = I->getParent()->getName().str();
    doAliasReasoning(&(*I), WorkSet, AliasedProgramVars);

    auto succs = getSuccessors(&(*I));
    branchInstructionMap[branchName].branch.insert(&(*I));
    for (auto succ : succs) {
      branchInstructionMap[branchName].successors.insert(succ);
    }
  }

  CFG TopCFG;
  buildCFG(TopCFG, realBranchOrder, branchInstructionMap);

  calledMethods.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions);
  calledMethods.setCFG(&TopCFG);
  calledMethods.setProgramVariables(AliasedProgramVars);

  mustCall.setFunctions(SafeFunctions, ReallocFunctions, MemoryFunctions);
  mustCall.setCFG(&TopCFG);
  mustCall.setProgramVariables(AliasedProgramVars);

  MappedMethods PostCalledMethods = calledMethods.generatePassResults();
  MappedMethods PostMustCalls = mustCall.generatePassResults();

  logout("\n\nPROGRAM VARS") for (ProgramVariable pv :
                                  AliasedProgramVars.getProgramVariables()) {
    logout("var raw name = " << pv.getRawName()) for (auto s :
                                                      pv.getAllAliases(false)) {
      logout(">> alias raw name = " << s)
    }
  }

  logout("\n\nPOST CALLED METHODS") for (auto Pair1 : PostCalledMethods) {
    std::string branchName = Pair1.first;
    logout("branch = " << branchName) for (auto Pair2 : Pair1.second) {
      std::string cm;
      for (auto m : Pair2.second.methodsSet) {
        cm += m + ", ";
      }
      logout(">> var name = " << Pair2.first << " cm = " << cm)
    }
  }

  logout("\n\nPOST MUST CALLS") for (auto Pair1 : PostMustCalls) {
    std::string branchName = Pair1.first;
    logout("branch = " << branchName) for (auto Pair2 : Pair1.second) {
      std::string mc;
      for (auto m : Pair2.second.methodsSet) {
        mc += m + ", ";
      }
      logout(">> var name = " << Pair2.first << " mc = " << mc)
    }
  }

  errs() << "\n\nRUNNING CALLED METHODS TESTS - "
         << " TEST NAME - " << testName << "\n\n";
  bool calledMethodsResult = TestRunner::runTests(
      calledMethods.getExpectedResult(), PostCalledMethods);

  errs() << "\n\nRUNNING MUST CALL TESTS "
         << " TEST NAME - " << testName << "\n\n";
  bool mustCallResult =
      TestRunner::runTests(mustCall.getExpectedResult(), PostMustCalls);

  if (calledMethodsResult == EXIT_SUCCESS && mustCallResult == EXIT_SUCCESS) {
    std::exit(EXIT_SUCCESS);
  } else {
    std::exit(EXIT_FAILURE);
  }
}

} // namespace dataflow