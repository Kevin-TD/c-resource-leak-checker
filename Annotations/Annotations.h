#define Annotate(x) __attribute__((annotate(x)))

#define MustCall(x) __attribute__((annotate("MustCall: {" #x "}")))
#define CalledMethods(x) __attribute__((annotate("CalledMethods: {" #x "}")))
#define MustCallUnknown __attribute__((annotate("MustCallUnknown: {}")))

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