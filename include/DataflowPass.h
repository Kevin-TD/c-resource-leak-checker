#ifndef DATAFLOW_PASS_H
#define DATAFLOW_PASS_H

#include "Annotations/Annotation.h"
#include "Annotations/AnnotationHandler.h"
#include "Annotations/ErrorAnnotation.h"
#include "Annotations/FunctionAnnotation.h"
#include "Annotations/ParameterAnnotation.h"
#include "Annotations/ReturnAnnotation.h"
#include "Annotations/StructAnnotation.h"
#include "CFG.h"
#include "ProgramRepresentation/FullFile.h"
#include "RunAnalysis.h"
#include "Utils.h"

class DataflowPass {
  private:
    void analyzeCFG(CFG *cfg, ProgramFunction &preProgramFunction,
                    ProgramFunction &postProgramFunction,
                    std::string priorBranch);
    void transfer(Instruction *instruction, ProgramPoint &inputProgramPoint);
    void insertAnnotation(Annotation *annotation, PVAliasSet *pvas, const std::string& invokerFnName);

    // overload that does not specify invoker function name.
    // needed for cases where specifying invoker name is incorrect.
    // see comment on DataflowPass::onAnnotation
    void insertAnnotation(Annotation *annotation, PVAliasSet *pvas);

    // a helper function that handles functions with Sret attribute.
    // returns true if the function had an Sret attribute and was handled,
    // and false if not.
    /*
      LLVM uses sret attributes as an optimization if the return type is
      large. a struct with more than 2 fields can trigger this optimization.

      docs: https://llvm.org/docs/LangRef.html#parameter-attributes
      less credible forum post:
      https://discourse.llvm.org/t/optimizing-sret-on-caller-side/60660

      our instructions end up looking like this:
      call void @does_something_a(%struct.my_struct* sret %B,
      %struct.my_struct* byval align 8 %A), !dbg !58

      here's how we will handle these types of instructions:

      say we have the instruction
      call void @does_something_a(%struct.my_struct* sret %B,
      %struct.my_struct* byval align 8 %A), !dbg !58

      (this is from C code
      my_struct B = does_something_a(A);)

      *we assume LHS is always a struct and first argument always has sret
      attribute

      we will:
      -destructure B into its fields (B.0, B.1, ...)
      -determine B's MustCall/CalledMethods from annotations on return on
      does_something_a
      -destructure A into its fields (A.0, A.1, ...)
      -determine A's MustCall/CalledMethods from annotations on params on
      does_something a past the first argument, it could be struct or
      non-struct. non-structs will have their MustCall/CalledMethods
      determined, but without the destructuring
    */
    bool handleSretCallForCallInsts(CallInst *call, int argIndex,
                                    const std::string &fnName,
                                    const std::string &argName,
                                    ProgramPoint &programPoint);

    // a helper function that handles function calls that are implicit,
    // or  identified as a memory, realloc, or safe function (/Functions files).
    // also handles when llvm debug function is called. returns true if we
    // handled the call, and false otherwise.
    bool handleIfKnownFunctionForCallInsts(CallInst *call, PVAliasSet *pvas);

    // a helper function that checks for parameter annotations on call
    // instructions. returns true if an annotation was found, and false if not.
    bool handleIfAnnotationExistsForCallInsts(const std::string &fnName,
            int argIndex, PVAliasSet *pvas);

  protected:
    ProgramFunction programFunction;
    AnnotationHandler annotations;
    std::string testName;
    CFG *cfg;
    FullFile expectedResult;

    virtual void leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet) = 0;

    virtual void onAllocationFunctionCall(PVAliasSet* input,
                                          std::string &fnName) = 0;
    virtual void onDeallocationFunctionCall(PVAliasSet* input,
                                            std::string &fnName) = 0;
    virtual void onUnknownFunctionCall(PVAliasSet* input, std::string &fnName) = 0;
    virtual void onReallocFunctionCall(PVAliasSet* input,
                                       std::string &fnName) = 0;
    virtual void onSafeFunctionCall(PVAliasSet* input, std::string &fnName) = 0;

    // annoFnName is the name of the function inside the annotation
    // invokerFnName is the name of the function the annotation belongs to
    /*
    e.g.,
    void free0(void* p Calls("free")) {
      free(p);
    }

    here, annoFnName = "free" and invokerFnName = "free0"
    */
    virtual void onAnnotation(PVAliasSet* input, const std::string &annoFnName, const std::string& invokerFnName,
                              AnnotationType annotationType) = 0;

    // annofnName is the name of the function inside the parameters.
    // this overload exists (where we do not specify the function that invoked
    // the call) because there are cases where it is not appropriate to add
    // the invoker function to the called methods set
    /*
    Example: see the following code

    -----------
    struct my_struct {
        char* x MustCall("free");
        char* y MustCall("free");
    };

    my_struct Calls("free", "x") free_and_obligate(my_struct S Calls("free", "x")) {
      free(S.x);
      return S;
    }

    int main() {
      my_struct A;
      A.x = (char*)malloc(15);

      my_struct B = free_and_obligate(A);
    }
    -----------

    This code finds that there is a ReturnAnnotation on the function
    free_and_obligate and will add "free" to return.x's called methods. Here, B is
    set to that return value so B.x will have called methods {"free"}. But if we
    also say to add the invoker name to the called methods (free_and_obligate),
    then B.x's called methods will be {"free", "free_and_obligate"}. This is not
    correct, hence the need to be able to include or and not include
    the invoker name.

    The only cases where invoker name needs to be specified are with
    ParameterAnnotations & unknwon function
    calls for the called methods pass and for
    (as of the existence of Calls and MustCall annotations)


    Most of these cases look like this:

    -----------
    struct my_struct {
        char* x MustCall("free");
        char* y MustCall("free");
    };

    void does_free(char* s Calls("free")) {
      free(s);
    }

    int main() {
      char* a = (char*)malloc(15);
      does_free(a);
    }
    -----------

    Here, it is appropriate that the called methods
    of a = {"free", "does_free"}

    */
    virtual void onAnnotation(PVAliasSet* input, const std::string &annoFnName,
                              AnnotationType annotationType) = 0;

  public:
    void setFunctions(std::set<std::string> safeFunctions,
                      std::set<std::string> reallocFunctions,
                      std::map<std::string, std::string> memoryFunctions,
                      AnnotationHandler annotations);

    std::set<std::string> safeFunctions;
    std::set<std::string> reallocFunctions;
    std::map<std::string, std::string> memoryFunctions;
    std::string passName;

    ProgramFunction generatePassResults();

    void setCFG(CFG *cfg);
    void setExpectedResult(FullFile expectedResult);
    void setProgramFunction(ProgramFunction programFunction);
    void setAnnotations(AnnotationHandler annotations);

    FullFile getExpectedResult();
};

#endif