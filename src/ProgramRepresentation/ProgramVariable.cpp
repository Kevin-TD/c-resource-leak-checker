#include "ProgramRepresentation/ProgramVariable.h"
#include "Debug.h"

ProgramVariable::ProgramVariable() {
    this->setNumber = 0;
}

ProgramVariable::ProgramVariable(std::string cleanedName) {
    this->cleanedName = cleanedName;
    this->index = -1;
    this->setNumber = 0;
}

ProgramVariable::ProgramVariable(Value *value) {
    this->value = value;
    this->rawName = rlc_dataflow::variable(value);
    this->cleanedName = this->rawName;
    this->index = -1;
    this->setNumber = 0;
    this->fixNameAndIdentifier();
}

ProgramVariable::ProgramVariable(Value *value, int index) {
    this->value = value;
    this->rawName = rlc_dataflow::variable(value) + "." + std::to_string(index);
    this->index = index;
    this->cleanedName = this->rawName;
    this->setNumber = 0;
    this->fixNameAndIdentifier();
}

void ProgramVariable::fixNameAndIdentifier() {
    this->varIsIdentifier = true;
    if (this->cleanedName[0] == '%' || this->cleanedName[0] == '@') {
        this->cleanedName.erase(0, 1);
    } else {
        this->varIsIdentifier = false;
    }
}

std::string ProgramVariable::getRawName() const {
    return this->rawName;
}

std::string ProgramVariable::getCleanedName() {
    return this->cleanedName;
}

Value *ProgramVariable::getValue() {
    return this->value;
}

bool ProgramVariable::hasProgramName() {
    return this->value->hasName();
}

bool ProgramVariable::containsStructFieldVar() {
    return this->index != -1;
}

int ProgramVariable::getIndex() {
    return this->index;
}

bool ProgramVariable::isIdentifier() {
    return this->varIsIdentifier;
}

bool ProgramVariable::equalsValue(Value *otherValue) {
    return this->value == otherValue;
}

bool ProgramVariable::equalsCleanedName(std::string otherName) {
    return this->cleanedName.compare(otherName) == 0;
}

bool ProgramVariable::equalsRawName(std::string otherRawName) {
    return this->rawName.compare(otherRawName) == 0;
}

unsigned ProgramVariable::getSetNumber() {
    return this->setNumber;
}

void ProgramVariable::setSetNumber(unsigned setNumber) {
    this->setNumber = setNumber;
}