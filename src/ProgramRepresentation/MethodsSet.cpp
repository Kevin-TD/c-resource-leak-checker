#include "./ProgramRepresentation/MethodsSet.h"

MethodsSet::MethodsSet() {
    this->setIsUninit = true; 
}

MethodsSet::MethodsSet(std::set<std::string> methods) {
    this->setMethods(methods); 
}

void MethodsSet::setMethods(std::set<std::string> methods) {
    this->methods = methods; 
    this->setIsUninit = false; 
}

std::set<std::string> MethodsSet::getMethods() {
    return this->methods; 
}

bool MethodsSet::isUninit() {
    return this->setIsUninit; 
}

void MethodsSet::addMethod(std::string method) {
    this->methods.insert(method); 
    this->setIsUninit = false; 
}