#include "Annotations/FunctionAnnotation.h"

FunctionAnnotation::FunctionAnnotation(AnnotationType annotationType,
                                       std::set<std::string> annotationMethods,
                                       std::string specifierName) {
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->specifierName = specifierName;
    this->isVerified = false;
}

std::string FunctionAnnotation::generateStringRep() const {
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    return "@" + annoTypeString + " FunctionAnnotation Name = " + specifierName +
           " methods = " + annoMethodsString;
}

bool FunctionAnnotation::functionNameEquals(const std::string &functionName) const {
    return functionName.compare(this->specifierName) == 0;
}