#include "Annotations/ParameterAnnotation.h"

ParameterAnnotation::ParameterAnnotation(
    AnnotationType annotationType, std::set<std::string> annotationMethods,
    std::string specifierName, unsigned field, unsigned nthParameter) {
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->specifierName = specifierName;
    this->field = field;
    this->nthParameter = nthParameter;
}

std::string ParameterAnnotation::toString() const {
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    std::string fieldString;
    if (this->hasField()) {
        fieldString = "Field = " + std::to_string(this->field);
    }

    return "@" + annoTypeString +
           " ParameterAnnotation Function Name = " + this->specifierName +
           " Parameter = #" + std::to_string(this->nthParameter) + " " +
           fieldString + " methods = " + annoMethodsString;
}

bool ParameterAnnotation::nthParameterEquals(unsigned param) const {
    return this->nthParameter == param;
}

bool ParameterAnnotation::functionNameEquals(const std::string &functionName) const {
    return functionName.compare(this->specifierName) == 0;
}

bool ParameterAnnotation::fieldEquals(unsigned field) const {
    return field == this->field;
}

bool ParameterAnnotation::hasField() const {
    return this->field != -1;
}

int ParameterAnnotation::getField() const {
    return this->field;
}

unsigned ParameterAnnotation::getNthParameter() const {
    return this->nthParameter;
}