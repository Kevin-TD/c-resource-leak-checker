#ifndef FUNCTION_INFO
#define FUNCTION_INFO

#include "Utils.h"
#include "TempFileManager.h"

struct ParamInfo {
    unsigned index;
    std::string type;
};

class FunctionInfo {
  private:
    std::string functionName;
    std::vector<ParamInfo> parameters;
    std::string returnType;

  public:
    FunctionInfo(const std::string& functionName, const std::string& returnType);

    void addParameter(const unsigned index, const std::string type);

    // returns empty string if there is not parameter with specified index
    std::string getNthParamType(const unsigned index) const;

    unsigned getNumberOfParameters() const;
    std::string getFunctionName() const;
    std::string getReturnType() const;
};

#endif