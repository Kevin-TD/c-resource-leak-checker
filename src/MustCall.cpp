#include "MustCall.h"
#include "Constants.h"
#include "Debug.h"

MustCall::MustCall() { this->passName = MUST_CALL_PASS_NAME; }

// fnName refers to corresponding deallocation/free function instead of memory
// allocating function
void MustCall::onAllocationFunctionCall(MethodsSet &input,
                                        std::string &fnName) {
  input.addMethod(fnName);
}
void MustCall::onDeallocationFunctionCall(MethodsSet &input,
                                          std::string &fnName) {}
void MustCall::onUnknownFunctionCall(MethodsSet &input) {}
void MustCall::onReallocFunctionCall(MethodsSet &input,
                                     std::string &fnName) {}
void MustCall::onSafeFunctionCall(MethodsSet &input,
                                  std::string &fnName) {}

void MustCall::leastUpperBound(std::set<std::string> &preMethods,
                               std::set<std::string> &curMethods,
                               std::set<std::string> &result) {
  std::set_union(preMethods.begin(), preMethods.end(),
                 curMethods.begin(), curMethods.end(),
                 std::inserter(result, result.begin()));
                 
}
void MustCall::onAnnotation(MethodsSet &input, std::string &fnName,
                            AnnotationType annotationType) {
  if (annotationType == AnnotationType::MustCallAnnotation) {
    input.addMethod(fnName);
  }
}