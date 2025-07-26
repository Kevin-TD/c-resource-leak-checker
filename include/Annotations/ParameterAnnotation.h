#ifndef PARAMETER_ANNOTATION_H
#define PARAMETER_ANNOTATION_H

#include "Annotation.h"

// reflects annotations placed on function parameters by the user
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
    // TODO: this would be better described with a Maybe<unsigned> monad
    // but that sounds iffy to do in C++11. consider this later
    int field;

    // refers to the 0-indexed n-th position of the parameter in relation
    // to some function
    unsigned nthParameter;

  public:
    ParameterAnnotation(AnnotationType annotationType,
                        std::set<std::string> annotationMethods,
                        std::string specifierName, unsigned field, unsigned nthParameter);
    std::string toString() const;
    bool nthParameterEquals(unsigned param) const;
    bool functionNameEquals(const std::string &functionName) const;
    bool fieldEquals(unsigned field) const;
    bool hasField() const;

    // returns the field assuming `hasField` is true; iff it is not true, then
    // -1 is returned.
    int getField() const;

    unsigned getNthParameter() const;
};

#endif