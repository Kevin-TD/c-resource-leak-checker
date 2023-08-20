#include "Annotations/StructAnnotation.h"

StructAnnotation::StructAnnotation(AnnotationType annotationType,
                                   std::set<std::string> annotationMethods,
                                   std::string targetName, std::string field) {
  this->annotationType = annotationType;
  this->annotationMethods = annotationMethods;
  this->targetName = targetName;
  this->field = field;
  this->isVerified = false;
}

std::string StructAnnotation::generateStringRep() {
  std::string annoTypeString;
  std::string annoMethodsString = "{";

  if (annotationType == AnnotationType::CallsAnnotation) {
    annoTypeString = "Calls";
  } else if (annotationType == AnnotationType::MustCallAnnotation) {
    annoTypeString = "MustCall";
  }

  for (std::string method : this->annotationMethods) {
    annoMethodsString += method + ", ";
  }
  annoMethodsString += "}";

  std::string fieldString;
  if (this->field.size() > 0) {
    fieldString = "Field = " + this->field;
  }

  return "@" + annoTypeString +
         " StructAnnotation Struct Name = " + this->targetName + " " +
         fieldString + " methods = " + annoMethodsString;
}

bool StructAnnotation::structNameEquals(const std::string &structName) {
  return structName.compare(this->targetName) == 0;
}

bool StructAnnotation::fieldNameEquals(const std::string &fieldName) {
  return fieldName.compare(this->field) == 0;
}

std::string StructAnnotation::getFieldName() { return this->field; }