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

/*
manages annotations for a single file (a `FullFile` object).
implementation detail: getter functions have the return type
of `Annotation* (`A*`) because we want to express that,
when we want to get annotation of subtype `T`, we might get
error subtype `E` if we are unable to find the annotation given
the user's arguments. ideally, the return type would be
something like `T* | E*`, but `A*` is used due to
constraints with our current type hierarchy.

notes on public method parameters:
- `functionName` and `structName` should be as it appears in the user's C code.
- `paramaterIndex` is the 0-indexed parameter of the function.
- `fieldIndex` is 0-indexed parameter specifying, for a
function that returns a struct, which field.
note: a struct `S {x, y}` has indexing `x -> 0`, `y -> 1`.


TODO: using C++'s multiple inheritance, we could construct
specific error types `(TE)*` if we let TE inherit both T and E,
where T and E are both inherited from A.
*/
class AnnotationHandler {
  private:
    std::list<Annotation *> annotations;
    void addAnnotation(const std::string &rawAnnotationString);

  public:
    AnnotationHandler();
    void addAnnotations(const std::vector<std::string> &rawAnnotationStrings);

    Annotation *getFunctionAnnotation(const std::string &functionName) const;

    Annotation *getParameterAnnotation(const std::string &functionName,
                                       unsigned parameterIndex) const;
    Annotation *getParameterAnnotation(const std::string &functionName,
                                       unsigned parameterIndex, unsigned fieldIndex) const;
    // we return all parameter's `p` of function `f` if `p` has
    // a corresponding `ParameterAnnotation` with a field specified.
    std::vector<Annotation *>
    getAllParameterAnnotationsWithFields(const std::string &functionName) const;
    std::vector<Annotation *>

    // we return all parameter's `p` of function `f` if `p` has
    // a corresponding `ParameterAnnotation` with no field index specified.
    getAllParameterAnnotationsWithoutFields(const std::string &functionName) const;
    Annotation *getReturnAnnotation(const std::string &functionName) const;
    Annotation *getReturnAnnotation(const std::string &functionName, unsigned fieldIndex) const;
    Annotation *getStructAnnotation(const std::string &structName, unsigned fieldIndex) const;
};

#endif