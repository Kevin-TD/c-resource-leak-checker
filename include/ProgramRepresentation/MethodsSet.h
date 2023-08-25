#ifndef METHODS_SET_H
#define METHODS_SET_H

#include "RunAnalysis.h"

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

    static MethodsSet copy(MethodsSet methods); 

};

#endif