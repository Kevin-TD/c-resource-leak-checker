#ifndef METHODS_SET_H
#define METHODS_SET_H

#include "RunAnalysis.h"

// holds some set of methods, whether it be the set of called methods or must
// calls. all ProgramVariable's (PVs) have their own MethodsSet
class MethodsSet {
  private:
    bool setIsUninit;
    std::set<std::string> methods;

  public:
    MethodsSet();
    MethodsSet(std::set<std::string> methods);

    void setMethods(std::set<std::string> methods);
    void addMethod(std::string method);
    void clearMethods();

    bool equals(MethodsSet otherMethodsSet);

    bool isUninit();

    std::set<std::string> getMethods();
};

#endif