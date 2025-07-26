#include "MustCall.h"
#include "Constants.h"
#include "Debug.h"

MustCall::MustCall() {
    this->passName = MUST_CALL_PASS_NAME;
}

void MustCall::onAllocationFunctionCall(PVAliasSet *input,
                                        std::string &fnName) {
    input->addMethod(fnName);
}
void MustCall::onDeallocationFunctionCall(PVAliasSet *input,
        std::string &fnName) {}
void MustCall::onUnknownFunctionCall(PVAliasSet *input) {}
void MustCall::onReallocFunctionCall(PVAliasSet *input, std::string &fnName) {}
void MustCall::onSafeFunctionCall(PVAliasSet *input, std::string &fnName) {}

void MustCall::leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet) {
    preSet.methodsSetUnion(curMethodsSet);
}

void MustCall::onAnnotation(PVAliasSet* input, Annotation* annotation) {
    if (annotation->getAnnotationType() == AnnotationType::MustCallAnnotation) {
        auto annoMethods = annotation->getAnnotationMethods();
        for (std::string annoMethod : annoMethods) {
            input->addMethod(annoMethod);

        }
    }
}

void MustCall::onFunctionCall(PVAliasSet* input, std::string &fnName) {}
