#ifndef RETURN_ANNOTATION_H
#define RETURN_ANNOTATION_H

#include "Annotation.h"

// reflects an annotation made by the user on a function that specifies
// some annotation type is to be applied on the function's return
class ReturnAnnotation : public Annotation {
  private:
    // iff the return is a struct type, this refers to its index.
    // otherwise, this equals -1.
    // TODO: use maybe monad?
    int field;

  public:
    ReturnAnnotation(AnnotationType annotationType,
                     std::set<std::string> annotationMethods,
                     std::string specifierName, unsigned field);
    std::string toString() const;
    bool functionNameEquals(const std::string &functionName) const;
    bool fieldIndexEquals(unsigned field) const;
    bool hasField() const;

    // returns the field assuming `hasField` is true; iff it is not true, then
    // -1 is returned.
    int getField() const;
};

#endif