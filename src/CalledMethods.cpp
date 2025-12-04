#include "CalledMethods.h"
#include "Constants.h"
#include "Debug.h"

CalledMethods::CalledMethods() {
    this->passName = CALLED_METHODS_PASS_NAME;
}

void CalledMethods::onAllocationFunctionCall(PVAliasSet *input,
        std::string &fnName) {}
void CalledMethods::onDeallocationFunctionCall(PVAliasSet *input,
        std::string &fnName) {
    input->addMethod(fnName);
}
void CalledMethods::onUnknownFunctionCall(PVAliasSet *input) {
    input->clearMethods();
}
void CalledMethods::onReallocFunctionCall(PVAliasSet *input,
        std::string &fnName) {
    input->clearMethods();
}
void CalledMethods::onSafeFunctionCall(PVAliasSet *input, std::string &fnName) {
}

void CalledMethods::leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet) {
    preSet.methodsSetIntersection(curMethodsSet);
}

void CalledMethods::onAnnotation(PVAliasSet* input, Annotation* annotation) {
    if (annotation->getAnnotationType() == AnnotationType::CallsAnnotation) {
        auto annoMethods = annotation->getAnnotationMethods();
        for (std::string annoMethod : annoMethods) {
            input->addMethod(annoMethod);
        }
    } else if(annotation->getAnnotationType() == AnnotationType::OwningAnnotation) {
        input->clearMethods();
    }
}

void CalledMethods::onFunctionCall(PVAliasSet* input, std::string &fnName) {
    input->addMethod(fnName);
}
