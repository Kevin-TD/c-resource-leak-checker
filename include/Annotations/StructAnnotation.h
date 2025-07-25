#ifndef STRUCT_ANNOTATION_H
#define STRUCT_ANNOTATION_H

#include "Annotation.h"

class StructAnnotation : public Annotation {
  private:
    int field;

  public:
    StructAnnotation(AnnotationType annotationType,
                     std::set<std::string> annotationMethods,
                     std::string specifierName, int field);
    std::string generateStringRep() const;
    bool structNameEquals(const std::string &structName) const;
    bool fieldNameEquals(int fieldName) const;

    int getFieldName() const;
};

#endif