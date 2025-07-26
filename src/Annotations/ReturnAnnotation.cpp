#include "Annotations/ReturnAnnotation.h"

ReturnAnnotation::ReturnAnnotation(AnnotationType annotationType,
                                   std::set<std::string> annotationMethods,
                                   std::string specifierName, int field) {
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->specifierName = specifierName;
    this->field = field;
}

std::string ReturnAnnotation::toString() const {
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    std::string fieldString;
    if (this->field != -1) {
        fieldString = "Field = " + std::to_string(this->field);
    }

    return "@" + annoTypeString +
           " ReturnAnnotation Function Name = " + this->specifierName + " " +
           fieldString + " methods = " + annoMethodsString;
}

bool ReturnAnnotation::fieldEquals(unsigned field) const {
    return field == this->field;
}

bool ReturnAnnotation::hasField() const {
    return this->field != -1;
}

bool ReturnAnnotation::functionNameEquals(const std::string &functionName) const {
    return functionName.compare(this->specifierName) == 0;
}

int ReturnAnnotation::getField() const {
    return this->field;
}