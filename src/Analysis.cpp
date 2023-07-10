#include "CFG.h"
#include "CalledMethods.h"
#include "DataflowPass.h"
#include "Debug.h"
#include "MustCall.h"
#include "RunAnalysis.h"
#include "TestRunner.h"
#include "Utils.h"

#include <fstream>
#include <list>
#include <map>
#include <tuple>
#include <vector>

// to run: cd build   then
// sh ../run_test.sh <test_num>
// or to run all tests: sh ../run_all.sh
// note for run all tests is that if you add more tests, you'll have to modify
// run_all.sh to include that test number

struct InstructionHolder {
  SetVector<Instruction *> branch;
  SetVector<Instruction *> successors;
};

namespace dataflow {

std::set<std::string> SafeFunctions;
std::set<std::string> UnsafeFunctions;
std::set<std::string> ReallocFunctions;
std::map<std::string, std::string> MemoryFunctions;
std::vector<std::string> realBranchOrder;
MappedMethods ExpectedResult;
bool ALLOW_REDEFINE; // ONLY for debugging purposes. exists so we can make our
                     // own functions without code saying it is a re-definition
                     // or clang saying it is undefined
bool loadAndBuild = false;
CalledMethods calledMethods;
MustCall mustCall;

void loadFunctions() {
  // working directory is /build

  std::ifstream safeFunctionsFile("../src/Functions/safe.txt");
  std::ifstream unsafeFunctionsFile("../src/Functions/unsafe.txt");
  std::ifstream reallocFunctionsFile("../src/Functions/realloc.txt");
  std::ifstream memoryFunctionsFile(
      "../src/Functions/test_memory.txt"); //*NOTE: test memory being used here
                                           // instead of "real" memory
                                           // allocation functions

  std::string line;
  if (safeFunctionsFile.is_open()) {
    while (std::getline(safeFunctionsFile, line)) {
      SafeFunctions.insert(line);
    }
  }

  if (unsafeFunctionsFile.is_open()) {
    while (std::getline(unsafeFunctionsFile, line)) {
      UnsafeFunctions.insert(line);
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
  unsafeFunctionsFile.close();
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

void populateAliasedVars(Instruction *instruction,
                         SetVector<Instruction *> &workSet,
                         AliasMap &aliasedVars) {
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

  if (auto Load = dyn_cast<LoadInst>(instruction)) {
    logout("(load) name is " << variable(Load) << " for "
                             << variable(Load->getPointerOperand()))
        std::string varName = variable(Load->getPointerOperand());
    while (varName.size() > 1 && isNumber(varName.substr(1))) {
      varName = aliasedVars[varName];
    }

    std::string loadName = dataflow::variable(Load);

    if (loadName[0] == '@') {
      loadName[0] = '%';
    }

    if (varName[0] == '@') {
      varName[0] = '%';
    }

    logout(loadName << " -> " << varName)

        aliasedVars[loadName] = varName;
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
    ALLOW_REDEFINE = TestRunner::getAllowedRedefine(testName);
    loadAndBuild = true;
  }

  if (!ALLOW_REDEFINE) {
    // check if code re-defines a safe function or memory function. if so, cast
    // it as a unsafe function
    if (SafeFunctions.count(fnName)) {
      SafeFunctions.erase(fnName);
      UnsafeFunctions.insert(fnName);
      functionIsKnown = true;
      errs() << "**ANALYSIS-WARNING**: Re-definition of safe function '"
             << fnName << "' identified and will be labelled as unsafe.\n";
    }

    for (auto Pair : MemoryFunctions) {
      if (fnName == Pair.first) {
        UnsafeFunctions.insert(fnName);
        functionIsKnown = true;
        errs() << "**ANALYSIS-WARNING**: Re-definition of allocation function '"
               << fnName << "' identified and will be labelled as unsafe.\n";
      }

      if (fnName == Pair.second) {
        UnsafeFunctions.insert(fnName);
        functionIsKnown = true;
        errs()
            << "**ANALYSIS-WARNING**: Re-definition of deallocation function '"
            << fnName << "' identified and will be labelled as unsafe.\n";
      }
    }
  }

  if (fnName == "main") {
    functionIsKnown = true;
  }

  if (!functionIsKnown && !ALLOW_REDEFINE) {
    errs() << "**ANALYSIS-WARNING**: Unknown function '" << fnName
           << "' identified and will be labelled as unsafe.\n";
    UnsafeFunctions.insert(fnName);
  }

  if (fnName != "main")
    return;

  AliasMap AliasedVars;
  std::map<std::string, InstructionHolder> branchInstructionMap;

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
    WorkSet.insert(&(*I));

    std::string branchName = I->getParent()->getName().str();
    populateAliasedVars(&(*I), WorkSet, AliasedVars);

    auto succs = getSuccessors(&(*I));
    branchInstructionMap[branchName].branch.insert(&(*I));
    for (auto succ : succs) {
      branchInstructionMap[branchName].successors.insert(succ);
    }
  }

  CFG TopCFG;
  buildCFG(TopCFG, realBranchOrder, branchInstructionMap);

  calledMethods.setFunctions(SafeFunctions, UnsafeFunctions, ReallocFunctions,
                             MemoryFunctions);
  calledMethods.setCFG(&TopCFG);
  calledMethods.setAliasedVars(AliasedVars);

  mustCall.setFunctions(SafeFunctions, UnsafeFunctions, ReallocFunctions,
                        MemoryFunctions);
  mustCall.setCFG(&TopCFG);
  mustCall.setAliasedVars(AliasedVars);

  MappedMethods PostCalledMethods = calledMethods.generatePassResults();
  MappedMethods PostMustCalls = mustCall.generatePassResults();

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

  errs() << "\n\nRUNNING CALLED METHODS TESTS - ALLOWED_REDEFINE = "
         << ALLOW_REDEFINE << " TEST NAME - " << testName << "\n\n";
  bool calledMethodsResult = TestRunner::runTests(
      calledMethods.getExpectedResult(), PostCalledMethods);

  errs() << "\n\nRUNNING MUST CALL TESTS - ALLOWED_REDEFINE = "
         << ALLOW_REDEFINE << " TEST NAME - " << testName << "\n\n";
  bool mustCallResult =
      TestRunner::runTests(mustCall.getExpectedResult(), PostMustCalls);

  if (calledMethodsResult == EXIT_SUCCESS && mustCallResult == EXIT_SUCCESS) {
    std::exit(EXIT_SUCCESS);
  } else {
    std::exit(EXIT_FAILURE);
  }
}

} // namespace dataflow