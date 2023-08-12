#ifndef PARAMETER_ANNOTATION_H
#define PARAMETER_ANNOTATION_H

#include "Annotation.h"

class ParameterAnnotation : public Annotation {
private:
  std::string field;
  int nthParameter;
  bool hasField;

public:
  ParameterAnnotation(AnnotationType annotationType,
                      std::set<std::string> annotationMethods,
                      std::string targetName, std::string field,
                      int nthParameter);
  std::string generateStringRep();
  bool nthParameterEquals(int param);
  bool functionNameEquals(const std::string &functionName);
  bool fieldNameEquals(const std::string &field);
  bool paramHasField();

  std::string getField();
  int getNthParameter();
};

#endif