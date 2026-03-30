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

std::set<std::string> MethodsSet::getMethods() const {
    return this->methods;
}

bool MethodsSet::isUninit() const {
    return this->setIsUninit;
}

bool MethodsSet::equals(MethodsSet otherMethodsSet) const {
    if (this->isUninit() != otherMethodsSet.isUninit()) {
        return false;
    }

    return this->getMethods() == otherMethodsSet.getMethods();
}

void MethodsSet::addMethod(std::string method) {
    this->methods.insert(method);
    this->setIsUninit = false;
}

void MethodsSet::clearMethods() {
    this->methods.clear();
    this->setIsUninit = false;
}
