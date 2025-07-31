#include "ProgramRepresentation/PVAliasSet.h"
#include "Constants.h"
#include "Debug.h"

PVAliasSet::PVAliasSet() {}

bool PVAliasSet::contains(const ProgramVariable &programVar) {
    for (ProgramVariable &pv : programVariables) {
        if (pv.getRawName() == programVar.getRawName()) {
            return true;
        }
    }

    return false;
}

bool PVAliasSet::contains(const std::string& cleanedName) {
    for (ProgramVariable &pv : programVariables) {
        if (pv.getCleanedName() == cleanedName) {
            return true;
        }
    }

    return false;
}

bool PVAliasSet::contains(Value* value) {
    for (ProgramVariable &pv : programVariables) {
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

std::string PVAliasSet::toString(bool cleanNames, bool includeSetNumber) const {
    std::string result = "{";
    int pvSize = programVariables.size();

    int iterator = 0;
    for (ProgramVariable pv : programVariables) {
        if (cleanNames) {
            result += pv.getCleanedName();
        } else {
            result += pv.getRawName();
        }

        if (includeSetNumber) {
            result += "-" + std::to_string(pv.getSetNumber());
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
            return pv.getFieldIndex();
        }
    }

    return -1;
}

bool PVAliasSet::containsCallInstVar() {
    for (ProgramVariable &pv : programVariables) {
        if (CallInst* call = dyn_cast<CallInst>(pv.getValue())) {
            std::string fnName = call->getCalledFunction()->getName().str();

            if (rlc_util::startsWith(fnName, LLVM_PTR_ANNOTATION) ||
                    rlc_util::startsWith(fnName, LLVM_VAR_ANNOTATION)) {
                continue;
            }

            return true;
        }
    }

    return false;
}

bool PVAliasSet::containsStructFieldVar() {
    for (ProgramVariable &pv : programVariables) {
        if (pv.containsStructFieldVar()) {
            return true;
        }
    }

    return false;
}

void PVAliasSet::changeSetNumbersBy(unsigned changeAmount) {
    for (ProgramVariable& pv : programVariables) {
        pv.setSetNumber(pv.getSetNumber() + changeAmount);
    }
}

unsigned PVAliasSet::getMaxSetNumber() {
    unsigned maxSetNumber = 0;

    for (ProgramVariable& pv : programVariables) {
        if (pv.getSetNumber() > maxSetNumber) {
            maxSetNumber = pv.getSetNumber();
        }
    }

    return maxSetNumber;
}

PVAliasSet PVAliasSet::moveOut(unsigned setNumber) {
    PVAliasSet removedVariables;

    auto it = programVariables.begin();
    while (it != programVariables.end()) {
        if (it->getSetNumber() == setNumber) {
            removedVariables.add(*it);
            it = programVariables.erase(it);
        } else {
            ++it;
        }
    }

    return removedVariables;
}

ProgramVariable PVAliasSet::moveOut(ProgramVariable pv) {
    ProgramVariable removedPV;

    auto it = programVariables.begin();
    while (it != programVariables.end()) {
        if (it->getRawName() == pv.getRawName()) {
            removedPV = *it;
            it = programVariables.erase(it);
        } else {
            ++it;
        }
    }

    return removedPV;
}