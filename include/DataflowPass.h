#ifndef DATAFLOW_PASS_H
#define DATAFLOW_PASS_H

#include "CFG.h"
#include "RunAnalysis.h"
#include "Utils.h"

#include <fstream>
#include <set>

// mapping between LLVM intermediate variables (e.g., %6, %7, %8) and variables
// visible in the code (e.g., %str). Used to find local must-aliases, though it
// is not expected to find all of them
typedef std::map<std::string, std::string> AliasMap;

struct MaybeUninitMethodsSet {
  std::set<std::string> methodsSet;
  bool setInitialized;
};

// mapping for an entire program; first string is branch name, second string is
// var name
typedef std::map<std::string, std::map<std::string, MaybeUninitMethodsSet>>
    MappedMethods;

class DataflowPass {
protected:
  AliasMap aliasedVars;
  std::string testName;
  CFG *cfg;
  MappedMethods expectedResult;

  void analyzeCFG(CFG *cfg, MappedMethods &PreMappedMethods,
                  MappedMethods &PostMappedMethods, std::string priorBranch);
  virtual void leastUpperBound(MaybeUninitMethodsSet &preMethods,
                               MaybeUninitMethodsSet &curMethods,
                               std::set<std::string> &result) = 0;

  void transfer(Instruction *instruction, SetVector<Instruction *> workSet,
                std::map<std::string, MaybeUninitMethodsSet> &inputMethodsSet);
  virtual void onAllocationFunctionCall(MaybeUninitMethodsSet &input,
                                        std::string &fnName) = 0;
  virtual void onDeallocationFunctionCall(MaybeUninitMethodsSet &input,
                                          std::string &fnName) = 0;
  virtual void onUnknownFunctionCall(MaybeUninitMethodsSet &input) = 0;
  virtual void onUnsafeFunctionCall(MaybeUninitMethodsSet &input,
                                    std::string &fnName) = 0;
  virtual void onReallocFunctionCall(MaybeUninitMethodsSet &input,
                                     std::string &fnName) = 0;
  virtual void onSafeFunctionCall(MaybeUninitMethodsSet &input,
                                  std::string &fnName) = 0;

public:
  void setFunctions(std::set<std::string> safeFunctions,
                    std::set<std::string> unsafeFunctions,
                    std::set<std::string> reallocFunctions,
                    std::map<std::string, std::string> memoryFunctions);

  std::set<std::string> safeFunctions;
  std::set<std::string> unsafeFunctions;
  std::set<std::string> reallocFunctions;
  std::map<std::string, std::string> memoryFunctions;
  std::string passName;

  MappedMethods generatePassResults();

  void setCFG(CFG *cfg);
  void setAliasedVars(AliasMap aliasedVars);
  void setExpectedResult(MappedMethods expectedResult);

  MappedMethods getExpectedResult();
};

#endif