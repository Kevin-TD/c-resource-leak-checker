#ifndef MUST_CALL_H
#define MUST_CALL_H

#include "DataflowPass.h"

class MustCall : public DataflowPass {
protected:
  void leastUpperBound(MaybeUninitMethodsSet &preMethods,
                       MaybeUninitMethodsSet &curMethods,
                       std::set<std::string> &result);
  void onAllocationFunctionCall(MaybeUninitMethodsSet &input,
                                std::string &fnName);
  void onDeallocationFunctionCall(MaybeUninitMethodsSet &input,
                                  std::string &fnName);
  void onUnknownFunctionCall(MaybeUninitMethodsSet &input);
  void onReallocFunctionCall(MaybeUninitMethodsSet &input, std::string &fnName);
  void onSafeFunctionCall(MaybeUninitMethodsSet &input, std::string &fnName);
  void onAnnotation(MaybeUninitMethodsSet &input, std::string &fnName,
                    AnnotationType annotationType);

public:
  MustCall();
};

#endif