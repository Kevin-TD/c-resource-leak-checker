#include "Annotations/ReturnAnnotation.h"

ReturnAnnotation::ReturnAnnotation(AnnotationType annotationType,
                                   std::set<std::string> annotationMethods,
                                   std::string specifierName, unsigned field) {
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->specifierName = specifierName;
    this->fieldIndex = fieldIndex;
}

std::string ReturnAnnotation::toString() const {
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    std::string fieldString;
    if (this->fieldIndex != -1) {
        fieldString = "Field = " + std::to_string(this->fieldIndex);
    }

    return "@" + annoTypeString +
           " ReturnAnnotation Function Name = " + this->specifierName + " " +
           fieldString + " methods = " + annoMethodsString;
}

bool ReturnAnnotation::fieldIndexEquals(unsigned fieldIndex) const {
    return fieldIndex == this->fieldIndex;
}

bool ReturnAnnotation::hasFieldIndex() const {
    return this->fieldIndex != -1;
}

bool ReturnAnnotation::functionNameEquals(const std::string &functionName) const {
    return functionName.compare(this->specifierName) == 0;
}

int ReturnAnnotation::getFieldIndex() const {
    return this->fieldIndex;
}