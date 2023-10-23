#ifndef DATAFLOW_PASS_H
#define DATAFLOW_PASS_H

#include "Annotations/Annotation.h"
#include "Annotations/AnnotationHandler.h"
#include "Annotations/ErrorAnnotation.h"
#include "Annotations/FunctionAnnotation.h"
#include "Annotations/ParameterAnnotation.h"
#include "Annotations/ReturnAnnotation.h"
#include "Annotations/StructAnnotation.h"
#include "CFG.h"
#include "ProgramRepresentation/FullFile.h"
#include "RunAnalysis.h"
#include "Utils.h"

class DataflowPass {
private:
  void analyzeCFG(CFG *cfg, ProgramFunction &preProgramFunction,
                  ProgramFunction &postProgramFunction,
                  std::string priorBranch);
  void transfer(Instruction *instruction, SetVector<Instruction *> workSet,
                ProgramPoint &inputProgramPoint);
  void insertAnnotation(Annotation* annotation, ProgramVariable* pv); 
  void handleSretCall(CallInst* call, const std::string& fnName, const std::string& argName, ProgramPoint& programPoint);

protected:
  ProgramFunction programFunction;
  AnnotationHandler annotations;
  std::string testName;
  CFG *cfg;
  FullFile expectedResult;

  virtual void leastUpperBound(MethodsSet &preMethods, MethodsSet &curMethods,
                               MethodsSet &result) = 0;

  virtual void onAllocationFunctionCall(MethodsSet *input,
                                        std::string &fnName) = 0;
  virtual void onDeallocationFunctionCall(MethodsSet *input,
                                          std::string &fnName) = 0;
  virtual void onUnknownFunctionCall(MethodsSet *input) = 0;
  virtual void onReallocFunctionCall(MethodsSet *input,
                                     std::string &fnName) = 0;
  virtual void onSafeFunctionCall(MethodsSet *input, std::string &fnName) = 0;
  virtual void onAnnotation(MethodsSet *input, std::string &fnName,
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

  ProgramFunction generatePassResults();

  void setCFG(CFG *cfg);
  void setExpectedResult(FullFile expectedResult);
  void setProgramFunction(ProgramFunction programFunction);
  void setAnnotations(AnnotationHandler annotations);

  FullFile getExpectedResult();
};

#endif