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

// manages annotations for a single file
class AnnotationHandler {
private:
  std::list<Annotation *> annotations;
  void addAnnotation(const std::string &rawAnnotationString);

public:
  AnnotationHandler();
  void addAnnotations(std::vector<std::string> rawAnnotationStrings);

  // returns Annotation* instead of more specific type (in this case,
  // FunctionAnnotation*) since we may not find the annotation and will want to
  // return ErrorAnnotation* instead
  /*
  TODO: C++ has multiple inheritance, so we could also fix this issue by
changing the type hierarchy. In particular, we could add types like
FunctionErrorAnnotation that are both a FunctionAnnotation and an
ErrorAnnotation; that is, make the type hierarchy look like this:

         Annotation
       /                   \
ErrorAnno        FunctionAnno
      \                     /
    FunctionErrorAnno
(and then we'd do the same for parameter annotations, etc.)


  */
  Annotation *getFunctionAnnotation(const std::string &functionName);

  Annotation *getParameterAnnotation(const std::string &functionName,
                                     int nthParameter);
  Annotation *getParameterAnnotation(const std::string &functionName,
                                     int nthParameter,
                                     int field);

  Annotation *getReturnAnnotation(const std::string &functionName);
  Annotation *getReturnAnnotation(const std::string &functionName,
                                  int field);

  Annotation *getStructAnnotation(const std::string &structName, int field);
};

#endif