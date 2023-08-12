#ifndef ANNOTATION_HANDLER_H
#define ANNOTATION_HANDLER_H

#include "Annotation.h"
#include "Debug.h"
#include "ErrorAnnotation.h"
#include "FunctionAnnotation.h"
#include "ParameterAnnotation.h"
#include "ReturnAnnotation.h"
#include "StructAnnotation.h"
#include "Utils.h"

class AnnotationHandler {
private:
  std::list<Annotation *> annotations;

public:
  AnnotationHandler();
  AnnotationHandler(const std::string &fileName);
  void addAnnotation(const std::string &rawAnnotationString);
  void addAnnotationsFromFile(const std::string &fileName);

  // returns Annotation* instead of more specific type (in this case,
  // FunctionAnnotation*) since we may not find the annotation and will want to
  // return ErrorAnnotation* instead
  Annotation *getFunctionAnnotation(const std::string &functionName);

  Annotation *getParameterAnnotation(const std::string &functionName,
                                     int nthParameter);
  Annotation *getParameterAnnotation(const std::string &functionName,
                                     int nthParameter,
                                     const std::string &field);

  Annotation *getReturnAnnotation(const std::string &functionName);
  Annotation *getReturnAnnotation(const std::string &functionName,
                                  const std::string &field);

  Annotation *getStructAnnotation(const std::string &structName,
                                  const std::string &field);
};

#endif