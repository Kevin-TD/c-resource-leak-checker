#ifndef MUST_CALL_H
#define MUST_CALL_H

#include "DataflowPass.h"

class MustCall : public DataflowPass {
  protected:
    void leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet);
    void onAllocationFunctionCall(PVAliasSet* input, std::string &fnName);
    void onDeallocationFunctionCall(PVAliasSet* input, std::string &fnName);
    void onUnknownFunctionCall(PVAliasSet* input, std::string &fnName);
    void onReallocFunctionCall(PVAliasSet* input, std::string &fnName);
    void onSafeFunctionCall(PVAliasSet* input, std::string &fnName);
    void onAnnotation(PVAliasSet* input, const std::string &annoFnName, const std::string& invokerFnName,
                      AnnotationType annotationType);
    void onAnnotation(PVAliasSet* input, const std::string &annoFnName,
                      AnnotationType annotationType);

  public:
    MustCall();
};

#endif