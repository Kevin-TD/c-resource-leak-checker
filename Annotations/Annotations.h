#define MustCall(target, method)                                               \
  __attribute__((annotate("TOOL_CHECKER MustCall " target " " method)))
#define Calls(target, methods)                                                 \
  __attribute__((annotate("TOOL_CHECKER Calls " target " " methods)))

#define METHODS(m) "methods = " m

#define FUNCTION(functionName) "target = FUNCTION(" functionName ")"
#define AT_PARAMETER(nthParameter) ".PARAM(" nthParameter ")"
#define AT_RETURN ".RETURN"
#define STRUCT(structName) "target = STRUCT(" structName ")"

#define AT_FIELD(target) ".FIELD(" target ")"

// unsure how much we'll be needing these
#define Owning __attribute__((annotate("Owning")))
#define MustCallAlias                                                          \
  __attribute__((                                                              \
      annotate("MustCallAlias"))) // implementation will be tricky  since this
                                  // needs a pair of annotations, one on the
                                  // return type and one for a parameter

// we need to be able to give annotations for an entire function or a paramter,
// right?

// note: annotations for parameters are inside the method via a call to `call
// void @llvm.var.annotation` annotations for functions are in
// @llvm.global.annotations