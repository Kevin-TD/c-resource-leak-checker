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

void MustCall::leastUpperBound(MethodsSet &preMethods,
                               MethodsSet &curMethods,
                               MethodsSet &result) {
  std::set<std::string> res;

  std::set_union(preMethods.getMethods().begin(), preMethods.getMethods().end(),
                 curMethods.getMethods().begin(), curMethods.getMethods().end(),
                 std::inserter(res, res.begin()));
                 
  result.setMethods(res);
}
void MustCall::onAnnotation(MethodsSet &input, std::string &fnName,
                            AnnotationType annotationType) {
  if (annotationType == AnnotationType::MustCallAnnotation) {
    input.addMethod(fnName);
  }
}