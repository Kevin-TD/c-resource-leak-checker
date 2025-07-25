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
    void addAnnotations(const std::vector<std::string> &rawAnnotationStrings);

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
    Annotation *getFunctionAnnotation(const std::string &functionName) const;

    Annotation *getParameterAnnotation(const std::string &functionName,
                                       int nthParameter) const;
    Annotation *getParameterAnnotation(const std::string &functionName,
                                       int nthParameter, int field) const;
    std::vector<Annotation *>
    getAllParameterAnnotationsWithFields(const std::string &functionName) const;
    std::vector<Annotation *>
    getAllParameterAnnotationsWithoutFields(const std::string &functionName) const;
    Annotation *getReturnAnnotation(const std::string &functionName) const;
    Annotation *getReturnAnnotation(const std::string &functionName, int field) const;
    Annotation *getStructAnnotation(const std::string &structName, int field) const;
};

#endif