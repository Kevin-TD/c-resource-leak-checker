#ifndef FUNCTION_ANNOTATION_H
#define FUNCTION_ANNOTATION_H

#include "Annotation.h"

// reflects annotations placed on functions by the user
class FunctionAnnotation : public Annotation {
  public:
    FunctionAnnotation(AnnotationType annotationType,
                       std::set<std::string> annotationMethods,
                       std::string specifierName);
    std::string toString() const;
    bool functionNameEquals(const std::string &functionName) const;
};

#endif