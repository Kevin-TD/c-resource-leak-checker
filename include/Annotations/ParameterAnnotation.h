#ifndef PARAMETER_ANNOTATION_H
#define PARAMETER_ANNOTATION_H

#include "Annotation.h"

class ParameterAnnotation : public Annotation {
  private:
    // iff the parameter is a struct, this refers to the
    // index of that struct. e.g., for a
    // struct my_struct { int x, y; }
    // and function
    // int foo(my_struct m Calls("free", "x"))
    // the field would be 0 for the 1st argument.
    // if it were Calls("free", "y"), the field would be 1.
    // if there was no struct, this would equal -1.
    int field;

    int nthParameter;

  public:
    ParameterAnnotation(AnnotationType annotationType,
                        std::set<std::string> annotationMethods,
                        std::string specifierName, int field, int nthParameter);
    std::string generateStringRep() const;
    bool nthParameterEquals(int param) const;
    bool functionNameEquals(const std::string &functionName) const;
    bool fieldNameEquals(int field) const;
    bool hasField() const;

    int getField() const;
    int getNthParameter() const;
};

#endif