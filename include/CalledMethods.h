#ifndef CALLED_METHODS_H
#define CALLED_METHODS_H

#include "DataflowPass.h"

class CalledMethods : public DataflowPass {
  protected:
    void leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet);
    void onAllocationFunctionCall(PVAliasSet* input, std::string &fnName);
    void onDeallocationFunctionCall(PVAliasSet* input, std::string &fnName);
    void onUnknownFunctionCall(PVAliasSet* input);
    void onReallocFunctionCall(PVAliasSet* input, std::string &fnName);
    void onSafeFunctionCall(PVAliasSet* input, std::string &fnName);
    void onAnnotation(PVAliasSet* input, std::string &fnName, AnnotationType annotationType);
    void onAnnotation(PVAliasSet* input, std::string &annoFnName, std::string& invokerFnName,
                      AnnotationType annotationType);

  public:
    CalledMethods();
};

#endif