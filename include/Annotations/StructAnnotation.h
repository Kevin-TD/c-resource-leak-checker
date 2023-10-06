#ifndef STRUCT_ANNOTATION_H
#define STRUCT_ANNOTATION_H

#include "Annotation.h"

class StructAnnotation : public Annotation {
private:
  int field;

public:
  StructAnnotation(AnnotationType annotationType,
                   std::set<std::string> annotationMethods,
                   std::string targetName, int field);
  std::string generateStringRep();
  bool structNameEquals(const std::string &structName);
  bool fieldNameEquals(int fieldName);

  int getFieldName();
};

#endif