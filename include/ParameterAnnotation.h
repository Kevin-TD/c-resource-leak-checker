#ifndef PARAMETER_ANNOTATION_H
#define PARAMETER_ANNOTATION_H

#include "Annotation.h"

class ParameterAnnotation : public Annotation {
private:
  std::string field;
  int nthParameter;

public:
  ParameterAnnotation(AnnotationType annotationType,
                      std::set<std::string> annotationMethods,
                      std::string targetName, std::string field,
                      int nthParameter);
  std::string generateStringRep();
};

#endif