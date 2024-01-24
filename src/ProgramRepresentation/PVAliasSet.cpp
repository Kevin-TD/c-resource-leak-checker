#include "ProgramRepresentation/PVAliasSet.h"

PVAliasSet::PVAliasSet() {}

bool PVAliasSet::contains(ProgramVariable programVar) {
    for (ProgramVariable pv : programVariables) {
        if (pv.getRawName() == programVar.getRawName()) {
            return true;
        }
    }

    return false;
}

bool PVAliasSet::contains(const std::string& cleanedName) {
    for (ProgramVariable pv : programVariables) {
        if (pv.getCleanedName() == cleanedName) {
            return true;
        }
    }

    return false;
}

bool PVAliasSet::contains(Value* value) {
    for (ProgramVariable pv : programVariables) {
        if (pv.getValue() == value) {
            return true;
        }
    }

    return false;
}

void PVAliasSet::add(ProgramVariable programVar) {
    if (contains(programVar)) {
        return;
    }

    programVariables.push_back(programVar);
}

void PVAliasSet::addProgramVariables(
    std::list<ProgramVariable> programVariables) {
    for (ProgramVariable pv : programVariables) {
        add(pv);
    }
}

std::list<ProgramVariable> PVAliasSet::getProgramVariables() {
    return programVariables;
}

MethodsSet PVAliasSet::getMethodsSet() const {
    return methods;
}

void PVAliasSet::methodsSetUnion(const MethodsSet other) {
    std::set<std::string> res;
    std::set<std::string> curSet = getMethodsSet().getMethods();
    std::set<std::string> otherSet = other.getMethods();

    std::set_union(curSet.begin(), curSet.end(), otherSet.begin(),
                   otherSet.end(), std::inserter(res, res.begin()));
    this->methods.setMethods(res);
}

void PVAliasSet::methodsSetIntersection(const MethodsSet other) {
    std::set<std::string> res;
    std::set<std::string> curSet = getMethodsSet().getMethods();
    std::set<std::string> otherSet = other.getMethods();

    std::set_intersection(curSet.begin(), curSet.end(), otherSet.begin(),
                          otherSet.end(), std::inserter(res, res.begin()));
    this->methods.setMethods(res);
}

void PVAliasSet::addMethod(std::string method) {
    this->methods.addMethod(method);
}

void PVAliasSet::clearMethods() {
    this->methods.clearMethods();
}

std::string PVAliasSet::toString(bool cleanNames) const {
    std::string result = "{";
    int pvSize = programVariables.size();

    int iterator = 0;
    for (ProgramVariable pv : programVariables) {
        if (cleanNames) {
            result += pv.getCleanedName();
        } else {
            result += pv.getRawName();
        }
        if (iterator != pvSize - 1) {
            result += ", ";
        }
        iterator++;
    }
    result += "}";
    return result;
}

std::string PVAliasSet::getMethodsString() const {
    std::set<std::string> methodsSet = methods.getMethods();
    return rlc_util::setToString(methodsSet);
}

int PVAliasSet::getIndex() {
    for (ProgramVariable pv : programVariables) {
        if (pv.containsStructFieldVar()) {
            return pv.getIndex();
        }
    }

    return -1;
}

bool PVAliasSet::containsStructFieldVar() {
    for (ProgramVariable pv : programVariables) {
        if (pv.containsStructFieldVar()) {
            return true;
        }
    }

    return false;
}