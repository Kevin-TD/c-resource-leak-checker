#ifndef FUNCTION_ANNOTATION_H
#define FUNCTION_ANNOTATION_H

#include "Annotation.h"

class FunctionAnnotation : public Annotation
{
public:
    FunctionAnnotation(AnnotationType annotationType,
                       std::set<std::string> annotationMethods,
                       std::string targetName);
    std::string generateStringRep();
    bool functionNameEquals(const std::string &functionName);
};

#endif