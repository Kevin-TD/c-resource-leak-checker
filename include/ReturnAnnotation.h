#ifndef RETURN_ANNOTATION_H
#define RETURN_ANNOTATION_H

#include "Annotation.h"

class ReturnAnnotation : public Annotation {
private:
  std::string field;
  bool hasField;

public:
  ReturnAnnotation(AnnotationType annotationType,
                   std::set<std::string> annotationMethods,
                   std::string targetName, std::string field);
  std::string generateStringRep();
  bool functionNameEquals(const std::string &functionName);
  bool fieldNameEquals(const std::string &field);
  bool returnHasField();

  std::string getField();
};

#endif