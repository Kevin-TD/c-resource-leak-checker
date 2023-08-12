#include "ReturnAnnotation.h"

ReturnAnnotation::ReturnAnnotation(AnnotationType annotationType,
                                   std::set<std::string> annotationMethods,
                                   std::string targetName, std::string field) {
  this->annotationType = annotationType;
  this->annotationMethods = annotationMethods;
  this->targetName = targetName;
  this->field = field;
  this->hasField = (field != "");
  this->isVerified = false;
}

std::string ReturnAnnotation::generateStringRep() {
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
         " ReturnAnnotation Function Name = " + this->targetName + " " +
         fieldString + " methods = " + annoMethodsString;
}

bool ReturnAnnotation::fieldNameEquals(const std::string &field) {
  return field.compare(this->field) == 0;
}

bool ReturnAnnotation::returnHasField() { return this->hasField; }

bool ReturnAnnotation::functionNameEquals(const std::string &functionName) {
  return functionName.compare(this->targetName) == 0;
}

std::string ReturnAnnotation::getField() { return this->field; }