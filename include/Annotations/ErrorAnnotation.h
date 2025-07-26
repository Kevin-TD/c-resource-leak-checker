#ifndef ERROR_ANNOTATION_H
#define ERROR_ANNOTATION_H

#include "Annotation.h"

// an ErrorAnnotation is returned if the input string to be recognized
// as an annotation fails at some point, whether semantically or by runtime.
// its toString method will not provide any useful information, other
// than that it should not be called.
// TODO: error when calling toString instead of producing an ignorable string?
class ErrorAnnotation : public Annotation {
  public:
    std::string toString() const;
};

#endif