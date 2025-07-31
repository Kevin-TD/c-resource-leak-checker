#ifndef FUNCTION_INFO_MANAGER_H
#define FUNCTION_INFO_MANAGER_H

#include "FunctionInfo.h"

// uses the AST of a file to get information about functions *not* from the point
// of view of the LLVM IR, but as it is seen in the code. this is important
// as the IR can desugar functions or perform optimizations which
// change the arity of those functions. this class enforces
// how the user wrote the function so we can properly analyze
// the resources they specified. e.g., let `struct S {x, y}`
// be a struct where `x` and `y` are resources, and let `f`
// be a function that takes one parameter `s` of type `S`; that is,
// `f` can be called via `f(S s)`. Say the user annotates
// parameter `s` as follows: `f(S s Calls("free", "y"))`.
// calling "free" on `s.y` is a well-defined operation. the IR, however
// may desugar the struct, and the annotation will remain
// unchanged, stuck onto the first parameter, so this turns into
// `f(s.x Calls("free", "y"), s.y)`. calling "free" on `s.x.y`
// is nonsensical and will error, but we know this is not what the
// user specified. therefore, we need this class to tell us
// what the user actually intended.
class FunctionInfosManager {
  private:
    std::vector<FunctionInfo> functionInfos;
  public:
    void buildFunctionInfo(const TempFileManager& astInfoFile);

    // returns NULL is function with specified name is not found
    FunctionInfo* getFunction(const std::string& functionName);

};

#endif