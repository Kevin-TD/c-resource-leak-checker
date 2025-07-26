#ifndef FUNCTION_INFO
#define FUNCTION_INFO

#include "Utils.h"
#include "TempFileManager.h"

// for some parameter of a function this struct is used to store
// the index (the 0-indexed position of the param) and its type
// as shown in the C code as a string literal here. e.g.,
// for function `f(int a, string b)`, the `int a` has
// `ParamInfo {0, int}` and `string b` has `ParamInfo {1, "string"}`
struct ParamInfo {
    unsigned index;
    std::string type;
};

// holds information about a function: it's name, parameters, and return type.
// parameter and return types are stored as string literals
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