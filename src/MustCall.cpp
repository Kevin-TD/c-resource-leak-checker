#include "MustCall.h"
#include "Constants.h"
#include "Debug.h"

MustCall::MustCall() { this->passName = MUST_CALL_PASS_NAME; }

void MustCall::onAllocationFunctionCall(MaybeUninitMethodsSet &input,
                                        std::string &fnName) {
  input.methodsSet.insert(fnName);
  input.setInitialized = true;
}
void MustCall::onDeallocationFunctionCall(MaybeUninitMethodsSet &input,
                                          std::string &fnName) {}
void MustCall::onUnknownFunctionCall(MaybeUninitMethodsSet &input) {}
void MustCall::onReallocFunctionCall(MaybeUninitMethodsSet &input,
                                     std::string &fnName) {}
void MustCall::onSafeFunctionCall(MaybeUninitMethodsSet &input,
                                  std::string &fnName) {}

void MustCall::leastUpperBound(MaybeUninitMethodsSet &preMethods,
                               MaybeUninitMethodsSet &curMethods,
                               std::set<std::string> &result) {
  std::set_union(preMethods.methodsSet.begin(), preMethods.methodsSet.end(),
                 curMethods.methodsSet.begin(), curMethods.methodsSet.end(),
                 std::inserter(result, result.begin()));
}