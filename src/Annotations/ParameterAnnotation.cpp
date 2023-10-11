#include "Annotations/ParameterAnnotation.h"

ParameterAnnotation::ParameterAnnotation(
    AnnotationType annotationType, std::set<std::string> annotationMethods,
    std::string targetName, std::string field, int nthParameter) {
  this->annotationType = annotationType;
  this->annotationMethods = annotationMethods;
  this->targetName = targetName;
  this->field = field;
  this->hasField = (field != "");
  this->nthParameter = nthParameter;
  this->isVerified = false;
}

std::string ParameterAnnotation::generateStringRep() {
  std::string annoTypeString = annotationTypeToString(this->annotationType);
  std::string annoMethodsString =
      dataflow::setToString(this->annotationMethods);

  std::string fieldString;
  if (this->field.size() > 0) {
    fieldString = "Field = " + this->field;
  }

  return "@" + annoTypeString +
         " ParameterAnnotation Function Name = " + this->targetName +
         " Parameter = #" + std::to_string(this->nthParameter) + " " +
         fieldString + " methods = " + annoMethodsString;
}

bool ParameterAnnotation::nthParameterEquals(int param) {
  return this->nthParameter == param;
}

bool ParameterAnnotation::functionNameEquals(const std::string &functionName) {
  return functionName.compare(this->targetName) == 0;
}

bool ParameterAnnotation::fieldNameEquals(const std::string &field) {
  return field.compare(this->field) == 0;
}

bool ParameterAnnotation::paramHasField() { return this->hasField; }

std::string ParameterAnnotation::getField() { return this->field; }

int ParameterAnnotation::getNthParameter() { return this->nthParameter; }