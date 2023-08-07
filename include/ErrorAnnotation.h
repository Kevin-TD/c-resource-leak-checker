#ifndef ERROR_ANNOTATION_H
#define ERROR_ANNOTATION_H

// this is not an annotation for an error but effectively takes the place of an
// error message used when we are checking if a raw annotation string is correct
// and turns out it is not

#include "Annotation.h"

class ErrorAnnotation : public Annotation {
public:
  std::string generateStringRep();
};

#endif