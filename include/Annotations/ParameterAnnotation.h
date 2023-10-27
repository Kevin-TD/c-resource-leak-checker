#ifndef PARAMETER_ANNOTATION_H
#define PARAMETER_ANNOTATION_H

#include "Annotation.h"

class ParameterAnnotation : public Annotation {
private:
  // equals -1 if not defined
  int field;

  int nthParameter;

  // returns true iff field does not equal -1
  bool hasField;

public:
  ParameterAnnotation(AnnotationType annotationType,
                      std::set<std::string> annotationMethods,
                      std::string targetName, int field, int nthParameter);
  std::string generateStringRep();
  bool nthParameterEquals(int param);
  bool functionNameEquals(const std::string &functionName);
  bool fieldNameEquals(int field);
  bool paramHasField();

  int getField();
  int getNthParameter();
};

#endif