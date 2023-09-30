#define CALLS_1_ARG(methods)                                                   \
  __attribute__((annotate("TOOL_CHECKER Calls target = _ methods "             \
                          "= " methods)))
#define CALLS_2_ARG(methods, field)                                            \
  __attribute__((annotate("TOOL_CHECKER Calls target = _.FIELD(" field         \
                          ") methods = " methods)))

#define GET_3RD_ARG(arg1, arg2, arg3, ...) arg3
#define CALLS_CHOOSER(...) GET_3RD_ARG(__VA_ARGS__, CALLS_2_ARG, CALLS_1_ARG, )

#define Calls(...) CALLS_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define MUST_CALL_1_ARG(methods)                                               \
  __attribute__((                                                              \
      annotate("TOOL_CHECKER MustCall target = _ methods = " methods)))
#define MUST_CALL_2_ARG(methods, field)                                        \
  __attribute__((annotate("TOOL_CHECKER MustCall target = _.FIELD(" field      \
                          ") methods = " methods)))

#define MUST_CALL_CHOOSER(...)                                                 \
  GET_3RD_ARG(__VA_ARGS__, MUST_CALL_2_ARG, MUST_CALL_1_ARG, )

#define MustCall(...) MUST_CALL_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

// TODO: add MustCallAlias and Owning annotation

// annotations have extra macros (CALLS_1_ARG, __VA_ARGS_, etc) so that we can
// support optional arguments. if a field must be specified, it turns an
// annotation from MustCall("x") to MustCall("x", "y") (must call x on field y)

// targets are initially _ or _.FIELD(...) since we do not know what the target
// is fully (function/struct name, @param or @return or @field). after the AST
// pass the target should be fully constructed