#include "FunctionInfo.h"

FunctionInfo::FunctionInfo(const std::string& functionName, const std::string& returnType) {
    this->functionName = functionName;
    this->returnType = returnType;
}

void FunctionInfo::addParameter(const unsigned index, const std::string type) {
    this->parameters.push_back({index, type});
}

std::string FunctionInfo::getNthParamType(const unsigned index) const {
    for (auto param : this->parameters) {
        if (param.index == index) {
            return param.type;
        }
    }

    return "";
}

unsigned FunctionInfo::getNumberOfParameters() const {
    return this->parameters.size();
}

std::string FunctionInfo::getFunctionName() const {
    return this->functionName;
}

std::string FunctionInfo::getReturnType() const {
    return this->returnType;
}