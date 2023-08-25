#include "./ProgramRepresentation/MethodsSet.h"
#include "Debug.h"

MethodsSet::MethodsSet() {
    this->setIsUninit = true; 
}

MethodsSet::MethodsSet(std::set<std::string> methods) {
    this->setMethods(methods); 
}

void MethodsSet::setMethods(std::set<std::string> methods) {
    this->methods.clear();
    this->methods = methods; 
    this->setIsUninit = false; 
}

std::set<std::string> MethodsSet::getMethods() {
    return this->methods; 
}

bool MethodsSet::isUninit() {
    return this->setIsUninit; 
}

bool MethodsSet::equals(MethodsSet otherMethodsSet) {
    if (this->isUninit() != otherMethodsSet.isUninit()) {
        return false;
    }

    if (this->getMethods() != otherMethodsSet.getMethods()) {
        return false; 
    }

    return true; 
}

void MethodsSet::addMethod(std::string method) {
    this->methods.insert(method); 
    this->setIsUninit = false; 
}

void MethodsSet::clearMethods() {
    this->methods.clear();
    this->setIsUninit = false; 
}

MethodsSet MethodsSet::copy(MethodsSet methods) {
    if (methods.isUninit()) {
        return MethodsSet(); 
    }
    
    std::set<std::string> methodsCopy(methods.getMethods()); 

    return MethodsSet(methodsCopy); 
}