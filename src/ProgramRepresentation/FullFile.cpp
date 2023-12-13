#include "ProgramRepresentation/FullFile.h"

FullFile::FullFile() {}

FullFile::FullFile(std::string fileName)
{
    this->fileName = fileName;
}

void FullFile::addProgramFunction(ProgramFunction programFunction)
{
    this->programFunctions.push_back(programFunction);
}

std::list<ProgramFunction> FullFile::getProgramFunctions()
{
    return this->programFunctions;
}

std::string FullFile::getFileName()
{
    return this->fileName;
}

ProgramFunction FullFile::getProgramFunction(std::string functionName,
        bool addNewIfNotFound)
{
    for (ProgramFunction &programFunction : this->programFunctions) {
        if (programFunction.getFunctionName() == functionName) {
            return programFunction;
        }
    }

    if (addNewIfNotFound) {
        ProgramFunction newProgramFunction = ProgramFunction(functionName);
        this->addProgramFunction(newProgramFunction);
        return newProgramFunction;
    }

    errs() << "Error at getProgramFunction: Program function not found and new "
           "program point not added\n";
    std::exit(EXIT_FAILURE);
}

ProgramFunction *FullFile::getProgramFunctionRef(std::string functionName,
        bool addNewIfNotFound)
{
    for (ProgramFunction &programFunction : this->programFunctions) {
        if (programFunction.getFunctionName() == functionName) {
            return &programFunction;
        }
    }

    if (addNewIfNotFound) {
        ProgramFunction newProgramFunction = ProgramFunction(functionName);
        this->addProgramFunction(newProgramFunction);
        return &this->programFunctions.back();
    }

    errs() << "Error at getProgramFunctionRef: Program function not found and "
           "new program point not added\n";
    std::exit(EXIT_FAILURE);
}