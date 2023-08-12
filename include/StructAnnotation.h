#ifndef STRUCT_ANNOTATION_H
#define STRUCT_ANNOTATION_H

#include "Annotation.h"

class StructAnnotation : public Annotation {
private:
  std::string field;

public:
  StructAnnotation(AnnotationType annotationType,
                   std::set<std::string> annotationMethods,
                   std::string targetName, std::string field);
  std::string generateStringRep();
  bool structNameEquals(const std::string &structName);
  bool fieldNameEquals(const std::string &fieldName);

  std::string getFieldName();
};

#endif