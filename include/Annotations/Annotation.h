#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "RunAnalysis.h"

enum AnnotationType {
  MustCallAnnotation,
  CallsAnnotation,
};

// TODO: add virtual method verifyAnnotation and implement for the superclasses

class Annotation {
protected:
  AnnotationType annotationType;
  std::set<std::string> annotationMethods;
  std::string targetName;
  bool isVerified;

public:
  bool annotationIsVerified();
  AnnotationType getAnnotationType();
  std::set<std::string> getAnnotationMethods();
  std::string getName();
  virtual std::string generateStringRep() = 0;
};

// more for debugging
std::string annotationTypeToString(AnnotationType anno);

Annotation *generateAnnotation(const std::string &rawAnnotationString);

bool methodsArgumentIsCorrectlyFormatted(const std::string &rawMethodsString);

bool rawStringIsCorrectlyFormatted(const std::string &rawAnnotationString);

#endif
