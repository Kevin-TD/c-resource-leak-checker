#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "RunAnalysis.h"

enum AnnotationType {
  MustCallAnnotation,
  CallsAnnotation,
};

class Annotation {
protected:
  AnnotationType annotationType;
  std::set<std::string> annotationMethods;
  std::string targetName;

public:
  virtual std::string generateStringRep() = 0;
};

Annotation *generateAnnotation(const std::string &rawAnnotationString);
bool methodsArgumentIsCorrectlyFormatted(const std::string &rawMethodsString);
bool rawStringIsCorrectlyFormatted(const std::string &rawAnnotationString);

#endif
