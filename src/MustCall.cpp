#include "MustCall.h"
#include "Constants.h"
#include "Debug.h"

MustCall::MustCall() {
    this->passName = MUST_CALL_PASS_NAME;
}

void MustCall::onAllocationFunctionCall(PVAliasSet *input,
                                        std::string &nameOfDeallocFunction) {
    input->addMethod(nameOfDeallocFunction);
}
void MustCall::onDeallocationFunctionCall(PVAliasSet *input,
        std::string &fnName) {}
void MustCall::onUnknownFunctionCall(PVAliasSet *input) {}

// Because we use the mem2reg pass, there are two cases of a realloc function, the argument
// that gets realloc'd and what it is stored into. This means that for the argument we can
// clear the obligations and for the result we store we need to set them

void MustCall::onReallocFunctionCall(PVAliasSet *result, PVAliasSet *arg, std::string &fnName) {
    arg->clearMethods(); // Argument is being realloc'd and no longer points to valid data
    // This might cause a problem if the analysis is expanded to have multiple
    // different kinds of obligations (i.e. free AND close)
    std::string free = "free";
    this->onAllocationFunctionCall(result, free);
    //free was hardcoded here for ease and can be changed later
}
void MustCall::onSafeFunctionCall(PVAliasSet *input, std::string &fnName) {}

void MustCall::leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet) {
    preSet.methodsSetUnion(curMethodsSet);
}
void MustCall::leastUpperBound(PVAliasSet *preSet, MethodsSet &curMethodsSet) {
    preSet->methodsSetUnion(curMethodsSet);
}


void MustCall::onAnnotation(PVAliasSet* input, Annotation* annotation) {
    if (annotation->getAnnotationType() == AnnotationType::MustCallAnnotation) {
        auto annoMethods = annotation->getAnnotationMethods();
        for (std::string annoMethod : annoMethods) {
            input->addMethod(annoMethod);

        }
    } else if(annotation->getAnnotationType() == AnnotationType::OwningAnnotation) {
        input->clearMethods();
    }
}

void MustCall::onFunctionCall(PVAliasSet* input, std::string &fnName) {}
