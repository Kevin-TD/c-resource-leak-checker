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
#include "FunctionInfosManager.h"
#include "ProgramRepresentation/FullFile.h"
#include "RunAnalysis.h"
#include "Utils.h"

// the dataflow pass is an abstract class that models the program flow.
// the class knows where to apply transfer and lub operations,
// but subtypes must provide the specific details on how those
// operations are done.
class DataflowPass {
  private:
    // looks at the cfg to analyze the flow between
    // the branches (nodes of the graph) and performs lub between
    // each branch and their successors.
    void analyzeCFG(CFG *cfg, ProgramFunction &preProgramFunction,
                    ProgramFunction &postProgramFunction,
                    const std::string &priorBranch);

    // analyses the instruction semantics and updates `inputProgramPoint`
    // accordingly, looking for function calls that an
    // implemented dataflow pass should handle
    void transfer(Instruction *instruction, ProgramPoint &inputProgramPoint);

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
    bool handleIfAnnotationExistsForCallInsts(const std::string &fnName, CallInst* call, PVAliasSet *pvas);

    // if the call argument is a struct ty, this function de-structures it into its fields and looks for annotations on those fields
    void handleIfStructTyAndIfFieldsHaveAnnotations(CallInst *call, unsigned argIndex, const std::string &fnName, const std::string &argName, ProgramPoint &programPoint, PVAliasSet* pvas);

  protected:
    ProgramFunction programFunction;
    Function *F;
    AnnotationHandler annotations;
    std::string optLoadFileName;
    CFG *cfg;
    FunctionInfosManager functionInfosManager;
    FullFile expectedResult;

    virtual void leastUpperBound(PVAliasSet &preSet, MethodsSet &curMethodsSet) = 0;

    virtual void onAllocationFunctionCall(PVAliasSet* input,
                                          std::string &fnName) = 0;
    virtual void onDeallocationFunctionCall(PVAliasSet* input,
                                            std::string &fnName) = 0;
    virtual void onUnknownFunctionCall(PVAliasSet* input) = 0;
    virtual void onReallocFunctionCall(PVAliasSet* input,
                                       std::string &fnName) = 0;
    virtual void onSafeFunctionCall(PVAliasSet* input, std::string &fnName) = 0;
    virtual void onFunctionCall(PVAliasSet* input, std::string &fnName) = 0;
    virtual void onAnnotation(PVAliasSet* input, Annotation* annotation) = 0;


  public:
    void setFunctions(std::set<std::string> safeFunctions,
                      std::set<std::string> reallocFunctions,
                      std::map<std::string, std::string> memoryFunctions,
                      AnnotationHandler annotations);

    std::set<std::string> safeFunctions;
    std::set<std::string> reallocFunctions;
    std::map<std::string, std::string> memoryFunctions;
    std::string passName;

    ProgramFunction *generatePassResults();

    void setCFG(CFG *cfg);
    void setFunc(llvm::Function *F);
    void setExpectedResult(FullFile expectedResult);
    void setProgramFunction(ProgramFunction programFunction);
    void setAnnotations(AnnotationHandler annotations);
    void setFunctionInfosManager(FunctionInfosManager functionInfosManager);
    void setOptLoadFileName(const std::string& optLoadFileName);

    FullFile getExpectedResult();
};

#endif
