#include "Annotations/ParameterAnnotation.h"

ParameterAnnotation::ParameterAnnotation(
    AnnotationType annotationType, std::set<std::string> annotationMethods,
    std::string specifierName, unsigned fieldIndex, unsigned parameterIndex) {
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->specifierName = specifierName;
    this->fieldIndex = fieldIndex;
    this->parameterIndex = parameterIndex;
}

std::string ParameterAnnotation::toString() const {
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    std::string fieldString;
    if (this->hasFieldIndex()) {
        fieldString = "Field = " + std::to_string(this->fieldIndex);
    }

    return "@" + annoTypeString +
           " ParameterAnnotation Function Name = " + this->specifierName +
           " Parameter = #" + std::to_string(this->parameterIndex) + " " +
           fieldString + " methods = " + annoMethodsString;
}

bool ParameterAnnotation::parameterIndexEquals(unsigned parameterIndex) const {
    return this->parameterIndex == parameterIndex;
}

bool ParameterAnnotation::functionNameEquals(const std::string &functionName) const {
    return functionName.compare(this->specifierName) == 0;
}

bool ParameterAnnotation::fieldIndexEquals(unsigned fieldIndex) const {
    return fieldIndex == this->fieldIndex;
}

bool ParameterAnnotation::hasFieldIndex() const {
    return this->fieldIndex != -1;
}

int ParameterAnnotation::getFieldIndex() const {
    return this->fieldIndex;
}

unsigned ParameterAnnotation::getParameterIndex() const {
    return this->parameterIndex;
}