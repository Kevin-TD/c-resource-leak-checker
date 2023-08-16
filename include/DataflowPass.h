#ifndef DATAFLOW_PASS_H
#define DATAFLOW_PASS_H

#include "Annotation.h"
#include "AnnotationHandler.h"
#include "CFG.h"
#include "ErrorAnnotation.h"
#include "FunctionAnnotation.h"
#include "ParameterAnnotation.h"
#include "ProgramVariablesHandler.h"
#include "ReturnAnnotation.h"
#include "RunAnalysis.h"
#include "StructAnnotation.h"
#include "Utils.h"

#include <fstream>
#include <set>

struct MaybeUninitMethodsSet {
  std::set<std::string> methodsSet;
  bool setInitialized;
};

// mapping for an entire program; first string is branch name, second string is
// var name
typedef std::map<std::string, std::map<std::string, MaybeUninitMethodsSet>>
    MappedMethods;

// mapping between function name and a MappedMethods
// TODO: just make this a class
// TODO?: make all test run at the very end

typedef std::map<std::string, MappedMethods> FunctionMappedMethods;

class DataflowPass {
protected:
  ProgramVariablesHandler programVariables;
  AnnotationHandler annotations;
  std::string testName;
  CFG *cfg;
  FunctionMappedMethods expectedResult;

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
  virtual void onReallocFunctionCall(MaybeUninitMethodsSet &input,
                                     std::string &fnName) = 0;
  virtual void onSafeFunctionCall(MaybeUninitMethodsSet &input,
                                  std::string &fnName) = 0;
  virtual void onAnnotation(MaybeUninitMethodsSet &input, std::string &fnName,
                            AnnotationType annotationType) = 0;

public:
  void setFunctions(std::set<std::string> safeFunctions,
                    std::set<std::string> reallocFunctions,
                    std::map<std::string, std::string> memoryFunctions,
                    AnnotationHandler annotations);

  std::set<std::string> safeFunctions;
  std::set<std::string> reallocFunctions;
  std::map<std::string, std::string> memoryFunctions;
  std::string passName;

  MappedMethods generatePassResults();

  void setCFG(CFG *cfg);
  void setExpectedResult(FunctionMappedMethods expectedResult);
  void setProgramVariables(ProgramVariablesHandler programVariables);
  void setAnnotations(AnnotationHandler annotations);

  FunctionMappedMethods getExpectedResult();
};

#endif