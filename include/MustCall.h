#ifndef MUST_CALL_H
#define MUST_CALL_H

#include "DataflowPass.h"

class MustCall : public DataflowPass {
protected:
  void leastUpperBound(MethodsSet &preMethods,
                       MethodsSet &curMethods,
                       MethodsSet &result);
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
  MustCall();
};

#endif