#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "RunAnalysis.h"
#include "Utils.h"

enum AnnotationType {
    MustCallAnnotation,
    CallsAnnotation,
};

// TODO: add virtual method verifyAnnotation and implement for the superclasses
// TODO: consider rewriting these anno string checks in the python
// annotation generator. the python annotation generator
// already checks things like if a struct being referred to
// actually exists in our program.

class Annotation {
  private:
    static bool
    rawStringIsCorrectlyFormatted(const std::string &rawAnnotationString);

  protected:
    AnnotationType annotationType;
    std::set<std::string> annotationMethods;
    std::string targetName;
    bool isVerified;

  public:
    static Annotation *generateAnnotation(const std::string &rawAnnotationString);
    bool annotationIsVerified();
    AnnotationType getAnnotationType();
    std::set<std::string> getAnnotationMethods();
    std::string getName();
    virtual std::string generateStringRep() = 0;
};

namespace rlc_annotation_util {

// more for debugging
std::string annotationTypeToString(AnnotationType anno);

} // namespace rlc_annotation_util

#endif
