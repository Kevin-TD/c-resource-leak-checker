#include "Annotations/StructAnnotation.h"

StructAnnotation::StructAnnotation(AnnotationType annotationType,
                                   std::set<std::string> annotationMethods,
                                   std::string specifierName, int field) {
    this->annotationType = annotationType;
    this->annotationMethods = annotationMethods;
    this->specifierName = specifierName;
    this->field = field;
}

std::string StructAnnotation::toString() const {
    std::string annoTypeString =
        rlc_annotation_util::annotationTypeToString(this->annotationType);
    std::string annoMethodsString =
        rlc_util::setToString(this->annotationMethods);

    std::string fieldString = "Field = " + std::to_string(this->field);

    return "@" + annoTypeString +
           " StructAnnotation Struct Name = " + this->specifierName + " " +
           fieldString + " methods = " + annoMethodsString;
}

bool StructAnnotation::structNameEquals(const std::string &structName) const {
    return structName.compare(this->specifierName) == 0;
}

bool StructAnnotation::fieldNameEquals(int fieldName) const {
    return fieldName == this->field;
}

int StructAnnotation::getFieldName() const {
    return this->field;
}