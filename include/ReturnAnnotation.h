#ifndef RETURN_ANNOTATION_H
#define RETURN_ANNOTATION_H

#include "Annotation.h"

class ReturnAnnotation : public Annotation {
private:
  std::string field;

public:
  ReturnAnnotation(AnnotationType annotationType,
                   std::set<std::string> annotationMethods,
                   std::string targetName, std::string field);
  std::string generateStringRep();
};

#endif