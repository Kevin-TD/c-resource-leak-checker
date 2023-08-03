#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "RunAnalysis.h"

enum AnnotationType {
  MustCallAnnotation,
  CallsAnnotation,
  UndefinedAnnotation
};

class Annotation {
protected:
  AnnotationType annotationType;
  std::set<std::string> annotationMethods;
  std::string targetName;

public:
  Annotation();
  bool annotationIsUndefined();
  virtual std::string generateStringRep() = 0;
};

Annotation *generateAnnotation(const std::string &rawAnnotationString);

#endif
