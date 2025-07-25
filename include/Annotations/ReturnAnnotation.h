#ifndef RETURN_ANNOTATION_H
#define RETURN_ANNOTATION_H

#include "Annotation.h"

class ReturnAnnotation : public Annotation {
  private:
    // if return is a struct type, this refers to its index.
    // otherwise, this equals -1.
    int field;

  public:
    ReturnAnnotation(AnnotationType annotationType,
                     std::set<std::string> annotationMethods,
                     std::string specifierName, int field);
    std::string generateStringRep() const;
    bool functionNameEquals(const std::string &functionName) const;
    bool fieldEquals(int field) const;
    bool hasField() const;

    int getField() const;
};

#endif