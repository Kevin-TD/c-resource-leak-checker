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
// occurred when parsing the user's annotation, whether it be a syntax
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
    -- StructAnnotation; always has FIELD extension
    */
    static bool
    rawStringIsCorrectlyFormatted(const std::string &rawAnnotationString);

  protected:
    AnnotationType annotationType;

    // set of methods specified in the user's annotation
    std::set<std::string> annotationMethods;

    // note the distinction between "specifier" and "specifier name".
    // an annotation on a function malloc has specifier `function` and name
    // `malloc`. so, in pseudo code, `malloc_anno.specifier = function` and
    // `malloc_anno.specifier_name = malloc`. for struct `my_struct`, similarly,
    // we'll have that `my_struct_anno.specifier = struct`
    // and `my_struct_anno.specifier_name = my_struct`.
    std::string specifierName;

  public:
    // produces an annotation given a well-formed string from the language annotations are
    // specified by, and an error annotation otherwise.
    // implementation detail: the return type is a pointer
    // so that we are allowed by the compiler to return any subtype of Annotation
    static Annotation *generateAnnotation(const std::string &rawAnnotationString);

    // annotation type is MustCall or Calls
    AnnotationType getAnnotationType() const;

    // gets the set of methods specified in the user's annotation
    std::set<std::string> getAnnotationMethods() const;

    // gets the specifier name (i.e., function name or struct name) of the
    // annotation
    std::string getSpecifierName() const;

    // string representation of the annotation that is implemented
    // uniquely for each annotation subtype
    virtual std::string toString() const = 0;
};

namespace rlc_annotation_util {

// turns the enum literal of AnnotationType into a string.
// this method is useful only really for us
// for debugging.
std::string annotationTypeToString(AnnotationType anno);

} // namespace rlc_annotation_util

#endif
