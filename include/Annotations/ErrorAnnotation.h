#ifndef ERROR_ANNOTATION_H
#define ERROR_ANNOTATION_H

// an ErrorAnnotation is returned when we check if a raw string
// is a valid annotation and it is not

#include "Annotation.h"

class ErrorAnnotation : public Annotation
{
public:
    std::string generateStringRep();
};

#endif