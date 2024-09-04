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

void MustCall::checkIfInputIsSubtypeOfAnnotation(PVAliasSet* input, Annotation* annotation, const std::string& infoOutputIfFail) {
    if (annotation->getAnnotationType() == AnnotationType::MustCallAnnotation) {
        std::set<std::string> annoMethods = annotation->getAnnotationMethods();
        this->checkIfInputIsSubtypeOfSet(input, annoMethods, infoOutputIfFail);
    }
}

void MustCall::checkIfInputIsSubtypeOfSet(PVAliasSet* input, std::set<std::string> setToCompareWith, const std::string& infoOutputIfFail) {
    std::set<std::string> pvasMethods = input->getMethodsSet().getMethods();

    if (pvasMethods > setToCompareWith) {
        std::set<std::string> setDifference = rlc_util::getSetDifference(pvasMethods, setToCompareWith);

        logout("ERROR: Input methods is not subtype for MustCall. Missing for " << infoOutputIfFail << ": ");
        logout(rlc_util::formatSet("MustCall(\"{}\")\n", setDifference));

    }
}