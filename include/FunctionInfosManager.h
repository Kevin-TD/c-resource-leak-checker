#ifndef FUNCTION_INFO_MANAGER_H
#define FUNCTION_INFO_MANAGER_H

#include "FunctionInfo.h"

class FunctionInfosManager {
  private:
    std::vector<FunctionInfo> functionInfos;
  public:
    void buildFunctionInfo(const TempFileManager& astInfoFile);

    // returns NULL is function with specified name is not found
    FunctionInfo* getFunction(const std::string& functionName);

};

#endif