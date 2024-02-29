#include "MustCall.h"
#include "Constants.h"
#include "Debug.h"

MustCall::MustCall() {
    this->passName = MUST_CALL_PASS_NAME;
}

// fnName refers to corresponding deallocation/free function instead of memory
// allocating function
void MustCall::onAllocationFunctionCall(PVAliasSet *input,
                                        std::string &fnName) {
    input->addMethod(fnName);
}
void MustCall::onDeallocationFunctionCall(PVAliasSet *input,
        std::string &fnName) {}
void MustCall::onUnknownFunctionCall(PVAliasSet *input, std::string &fnName) {}
void MustCall::onReallocFunctionCall(PVAliasSet *input, std::string &fnName) {}
void MustCall::onSafeFunctionCall(PVAliasSet *input, std::string &fnName) {}

void MustCall::leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet) {
    preSet.methodsSetUnion(curMethodsSet);
}

void MustCall::onAnnotation(PVAliasSet* input, Annotation* annotation, const std::string& invokerFnName) {
    if (annotation->getAnnotationType() == AnnotationType::MustCallAnnotation) {
        auto annoMethods = annotation->getAnnotationMethods();
        for (std::string annoMethod : annoMethods) {
            input->addMethod(annoMethod);

        }
    }
}