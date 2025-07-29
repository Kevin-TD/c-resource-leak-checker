#include "Annotations/StructAnnotation.h"

StructAnnotation::StructAnnotation(AnnotationType annotationType,
                                   std::set<std::string> annotationMethods,
                                   std::string specifierName, unsigned fieldIndex) {
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->specifierName = specifierName;
    this->fieldIndex = fieldIndex;
}

std::string StructAnnotation::toString() const {
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    std::string fieldString = "Field = " + std::to_string(this->fieldIndex);

    return "@" + annoTypeString +
           " StructAnnotation Struct Name = " + this->specifierName + " " +
           fieldString + " methods = " + annoMethodsString;
}

bool StructAnnotation::structNameEquals(const std::string &structName) const {
    return structName.compare(this->specifierName) == 0;
}

bool StructAnnotation::fieldIndexEquals(unsigned fieldIndex) const {
    return fieldIndex == this->fieldIndex;
}

unsigned StructAnnotation::getFieldIndex() const {
    return this->fieldIndex;
}