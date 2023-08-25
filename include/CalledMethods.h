#ifndef CALLED_METHODS_H
#define CALLED_METHODS_H

#include "DataflowPass.h"

class CalledMethods : public DataflowPass {
protected:
  void leastUpperBound(std::set<std::string> &preMethods,
                       std::set<std::string> &curMethods,
                       std::set<std::string> &result);
  void onAllocationFunctionCall(MethodsSet &input,
                                std::string &fnName);
  void onDeallocationFunctionCall(MethodsSet &input,
                                  std::string &fnName);
  void onUnknownFunctionCall(MethodsSet &input);
  void onReallocFunctionCall(MethodsSet &input, std::string &fnName);
  void onSafeFunctionCall(MethodsSet &input, std::string &fnName);
  void onAnnotation(MethodsSet &input, std::string &fnName,
                    AnnotationType annotationType);

public:
  CalledMethods();
};

#endif