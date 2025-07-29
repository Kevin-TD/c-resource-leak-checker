#ifndef MUST_CALL_H
#define MUST_CALL_H

#include "DataflowPass.h"

// implementation of the pass that computes an overestimate
// of the set of methods that must be called for each resource
class MustCall : public DataflowPass {
  protected:
    void leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet);

    // if we have a resource allocated by `f` and de-allocated by `g`,
    // upon any call to `f` we must provide `g` to enforce that
    // the resource must call `g` now, hence why the second
    // parameter is `nameOfDeallocFunction` and not `fnName`.
    void onAllocationFunctionCall(PVAliasSet* input, std::string &nameOfDeallocFunction);
    void onDeallocationFunctionCall(PVAliasSet* input, std::string &fnName);
    void onUnknownFunctionCall(PVAliasSet* input);
    void onReallocFunctionCall(PVAliasSet* input, std::string &fnName);
    void onSafeFunctionCall(PVAliasSet* input, std::string &fnName);
    void onAnnotation(PVAliasSet* input, Annotation* annotation);
    void onFunctionCall(PVAliasSet* input, std::string &fnName);

  public:
    MustCall();
};

#endif