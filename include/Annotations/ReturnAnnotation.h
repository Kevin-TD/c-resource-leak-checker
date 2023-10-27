#ifndef RETURN_ANNOTATION_H
#define RETURN_ANNOTATION_H

#include "Annotation.h"

class ReturnAnnotation : public Annotation {
private:
  // equals -1 if not defined
  int field;

  // returns true iff field does not equal -1
  bool hasField;

public:
  ReturnAnnotation(AnnotationType annotationType,
                   std::set<std::string> annotationMethods,
                   std::string targetName, int field);
  std::string generateStringRep();
  bool functionNameEquals(const std::string &functionName);
  bool fieldNameEquals(int field);
  bool returnHasField();

  int getField();
};

#endif