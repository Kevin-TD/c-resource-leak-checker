#ifndef RETURN_ANNOTATION_H
#define RETURN_ANNOTATION_H

#include "Annotation.h"

// reflects an annotation made by the user on a function that specifies
// some annotation type is to be applied on the function's return
class ReturnAnnotation : public Annotation {
  private:
    // iff the return is a struct type, this refers to its index.
    // otherwise, this equals -1.
    int field;

  public:
    ReturnAnnotation(AnnotationType annotationType,
                     std::set<std::string> annotationMethods,
                     std::string specifierName, int field);
    std::string toString() const;
    bool functionNameEquals(const std::string &functionName) const;
    bool fieldEquals(unsigned field) const;
    bool hasField() const;

    int getField() const;
};

#endif