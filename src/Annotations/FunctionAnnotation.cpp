#include "Annotations/FunctionAnnotation.h"

FunctionAnnotation::FunctionAnnotation(AnnotationType annotationType,
                                       std::set<std::string> annotationMethods,
                                       std::string targetName) {
  this->annotationType = annotationType;
  this->annotationMethods = annotationMethods;
  this->targetName = targetName;
  this->isVerified = false;
}

std::string FunctionAnnotation::generateStringRep() {
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

  return "@" + annoTypeString + " FunctionAnnotation Name = " + targetName +
         " methods = " + annoMethodsString;
}

bool FunctionAnnotation::functionNameEquals(const std::string &functionName) {
  return functionName.compare(this->targetName) == 0;
}