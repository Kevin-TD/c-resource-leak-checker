#include "ProgramRepresentation/ProgramFunction.h"
#include "Debug.h"
#include "ProgramRepresentation/ProgramVariable.h"

ProgramFunction::ProgramFunction(std::string functionName) {
    this->functionName = functionName;
}

ProgramFunction::ProgramFunction() {}

void ProgramFunction::setAnnotationHandler(AnnotationHandler a) {
    this->a = a;
}

AnnotationHandler *ProgramFunction::getAnnotationHandler() {
    return &a;
}

void ProgramFunction::addProgramBlock(ProgramBlock programBlock) {
    this->programBlocks.push_back(programBlock);
}

std::list<ProgramBlock> ProgramFunction::getProgramBlocks() const {
    return this->programBlocks;
}

ProgramBlock *ProgramFunction::getProgramBlockRef(const std::string &blockName,
        bool addNewIfNotFound) {
    for (ProgramBlock &programBlock : this->programBlocks) {
        if (programBlock.getBlockName() == blockName) {
            return &programBlock;
        }
    }

    if (addNewIfNotFound) {
        ProgramBlock newProgramBlock = ProgramBlock(blockName);
        this->addProgramBlock(newProgramBlock);
        return &this->programBlocks.back();
    }

    errs() << "Error at getProgramBlockRef: Program point not found and new "
           "program block not added\n";
    std::exit(EXIT_FAILURE);
}

ProgramBlock ProgramFunction::getProgramBlock(const std::string &blockName,
        bool addNewIfNotFound) {
    for (ProgramBlock programBlock : this->programBlocks) {
        if (programBlock.getBlockName() == blockName) {
            return programBlock;
        }
    }

    if (addNewIfNotFound) {
        ProgramBlock newProgramBlock = ProgramBlock(blockName);
        this->addProgramBlock(newProgramBlock);
        return this->programBlocks.back();
    }

    errs() << "Error at getProgramPoint: Program point not found and new program "
           "point not added\n";
    std::exit(EXIT_FAILURE);
}

std::string ProgramFunction::getFunctionName() const {
    return this->functionName;
}

void ProgramFunction::setProgramBlock(std::string name, ProgramBlock programBlock) {
    ProgramBlock *programBlockRef = this->getProgramBlockRef(name, true);
    programBlockRef->points = programBlock.points;
    /*programBlockRef->getPoint(0)->setProgramVariableAliasSets(
        programBlock.getPoint(0)->getProgramVariableAliasSets());
    */
}

/*
PVAliasSet *ProgramFunction::getPVASRefFromValue(Value* value) {
    for (ProgramPoint& programPoint : programPoints) {
        if (PVAliasSet* pvas = programPoint.getPVASRef(value, false)) {
            return pvas;
        }
    }

    return NULL;
}
*/

void ProgramFunction::logoutProgramFunction(ProgramFunction &programFunction,
        bool logMethods) {
    for (auto block : programFunction.getProgramBlocks()) {
        logout("\n**point name " << block.getBlockName());
        ProgramBlock::logoutProgramBlock(block);
    }
}
