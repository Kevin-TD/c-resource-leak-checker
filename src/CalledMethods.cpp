#include "CalledMethods.h"
#include "Constants.h"
#include "Debug.h"

CalledMethods::CalledMethods() { this->passName = CALLED_METHODS_PASS_NAME; }

void CalledMethods::onAllocationFunctionCall(MethodsSet &input,
                                             std::string &fnName) {}
void CalledMethods::onDeallocationFunctionCall(MethodsSet &input,
                                               std::string &fnName) {
  input.addMethod(fnName);
}
void CalledMethods::onUnknownFunctionCall(MethodsSet &input) {
  input.clearMethods(); 
}
void CalledMethods::onReallocFunctionCall(MethodsSet &input,
                                          std::string &fnName) {
  input.clearMethods(); 
}
void CalledMethods::onSafeFunctionCall(MethodsSet &input,
                                       std::string &fnName) {}
void CalledMethods::leastUpperBound(std::set<std::string> &preMethods,
                                    std::set<std::string> &curMethods,
                                    std::set<std::string> &result) {
  std::set_intersection(preMethods.begin(), preMethods.end(),
                 curMethods.begin(), curMethods.end(),
                 std::inserter(result, result.begin()));

}

void CalledMethods::onAnnotation(MethodsSet &input,
                                 std::string &fnName,
                                 AnnotationType annotationType) {
  if (annotationType == AnnotationType::CallsAnnotation) {
    input.addMethod(fnName);
  }
}