#ifndef STRUCT_ANNOTATION_H
#define STRUCT_ANNOTATION_H

#include "Annotation.h"

// reflects an annotation the user puts on some field of a struct
class StructAnnotation : public Annotation {
  private:
    unsigned field;

  public:
    StructAnnotation(AnnotationType annotationType,
                     std::set<std::string> annotationMethods,
                     std::string specifierName, unsigned field);
    std::string toString() const;
    bool structNameEquals(const std::string &structName) const;
    bool fieldIndexEquals(unsigned field) const;

    unsigned getField() const;
};

#endif