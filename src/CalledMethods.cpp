#include "CalledMethods.h"
#include "Constants.h"
#include "Debug.h"

CalledMethods::CalledMethods() {
    this->passName = CALLED_METHODS_PASS_NAME;
}

void CalledMethods::onAllocationFunctionCall(MethodsSet *input,
        std::string &fnName) {}
void CalledMethods::onDeallocationFunctionCall(MethodsSet *input,
        std::string &fnName) {
    input->addMethod(fnName);
}
void CalledMethods::onUnknownFunctionCall(MethodsSet *input) {
    input->clearMethods();
}
void CalledMethods::onReallocFunctionCall(MethodsSet *input,
        std::string &fnName) {
    input->clearMethods();
}
void CalledMethods::onSafeFunctionCall(MethodsSet *input, std::string &fnName) {
}
void CalledMethods::leastUpperBound(MethodsSet &preMethods,
                                    MethodsSet &curMethods,
                                    MethodsSet &result) {
    std::set<std::string> res;
    std::set<std::string> preSet = preMethods.getMethods();
    std::set<std::string> curSet = curMethods.getMethods();

    std::set_intersection(preSet.begin(), preSet.end(), curSet.begin(),
                          curSet.end(), std::inserter(res, res.begin()));
    result.setMethods(res);
}

void CalledMethods::onAnnotation(MethodsSet *input, std::string &fnName,
                                 AnnotationType annotationType) {
    if (annotationType == AnnotationType::CallsAnnotation) {
        input->addMethod(fnName);
    }
}