#ifndef FUNCTION_INFO_MANAGER_H
#define FUNCTION_INFO_MANAGER_H

#include "FunctionInfo.h"

// uses the AST of a file to get information about functions *not* from the point
// of view of the LLVM IR, but as it is seen in the code. this is important
// as the IR can desugar functions or perform optimizations which
// change the arity of those functions; if the arity is different that what
// the user actually sees, then we may (or may not) analyze resources
// the user did not (or did) specify
class FunctionInfosManager {
  private:
    std::vector<FunctionInfo> functionInfos;
  public:
    void buildFunctionInfo(const TempFileManager& astInfoFile);

    // returns NULL is function with specified name is not found
    FunctionInfo* getFunction(const std::string& functionName);

};

#endif