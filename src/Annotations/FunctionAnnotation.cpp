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
  std::string annoTypeString =
      AnnotationUtil::annotationTypeToString(this->annotationType);
  std::string annoMethodsString = Util::setToString(this->annotationMethods);

  return "@" + annoTypeString + " FunctionAnnotation Name = " + targetName +
         " methods = " + annoMethodsString;
}

bool FunctionAnnotation::functionNameEquals(const std::string &functionName) {
  return functionName.compare(this->targetName) == 0;
}