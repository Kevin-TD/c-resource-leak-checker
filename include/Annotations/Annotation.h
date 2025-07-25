#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "RunAnalysis.h"
#include "Utils.h"

// representation of all annotation types available to the user to add to their C code
enum AnnotationType {
    MustCallAnnotation,
    CallsAnnotation,
};

// TODO: add virtual method verifyAnnotation and implement for the superclasses
// TODO: consider rewriting these anno string checks in the python
// annotation generator. the python annotation generator
// already checks things like if a struct being referred to
// actually exists in our program.

// represents, generically, a piece of information (a specifier or an extension)
// a user can specify in an annotation. it is a base class for the
// Function & Struct annotations (specifiers), and
// Parameter & Return (extensions) annotations. the Field extension is tied
// to the Struct specifier, so it does not get its own class subtype.
// there is also the Error annotation, which is used to tell us that an error
// occurred when parsing the user's annotation, whether it be a semantic
// or runtime error.
class Annotation {
  private:
    /* checks that rawAnnotationString is in the language of valid annotations.
    raw annotations have the (pseudo) regex pattern of
    (Calls | MustCall)
    target = (
        (FUNCTION(<var name>)
            (.PARAM(<int>) | .RETURN))?
            .FIELD(<int>)?)
        | (STRUCT(<name>)).FIELD(<int>)
    )
    methods = <var name list>
    where '|' is xor and '?' refers to an optional extension

    note: FIELD extension refers to the index that belongs to some struct.
    e.g., struct S = {x, y}, S.x is index 0, and S.y is index 1.
    the AST pass converts names to indicies.

    examples:
    Calls target = FUNCTION(does_free) methods = free
    -- FunctionAnnotation; not anticipated that we'll be needing it but some
    handling for it is here just in case

    Calls target = FUNCTION(does_free).PARAM(1) methods = free
    -- ParameterAnnotation

    Calls target = FUNCTION(creates_obligation).PARAM(2).FIELD(0)
    methods = free
    -- ParameterAnnotation with field

    MustCall target = FUNCTION(creates_obligation).RETURN methods =
    free
    -- ReturnAnnotation

    Calls target = FUNCTION(does_something).RETURN.FIELD(0) methods =
    free
    -- ReturnAnnotation with field

    MustCall target = STRUCT(my_struct).FIELD(0) methods = free
    -- StructAnnotation; always has FIELD specifier
    */
    static bool
    rawStringIsCorrectlyFormatted(const std::string &rawAnnotationString);

  protected:
    AnnotationType annotationType;
    std::set<std::string> annotationMethods;
    std::string specifierName;
    bool isVerified;

  public:
    static Annotation *generateAnnotation(const std::string &rawAnnotationString);
    bool annotationIsVerified() const;
    AnnotationType getAnnotationType() const;
    std::set<std::string> getAnnotationMethods() const;
    std::string getSpecifierName() const;
    virtual std::string generateStringRep() const = 0;
};

namespace rlc_annotation_util {

// more for debugging
std::string annotationTypeToString(AnnotationType anno);

} // namespace rlc_annotation_util

#endif
