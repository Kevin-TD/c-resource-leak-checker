#include "ProgramRepresentation/ProgramFunction.h"
#include "Debug.h"
#include "ProgramRepresentation/ProgramVariable.h"

ProgramFunction::ProgramFunction(std::string functionName) {
    this->functionName = functionName;
    this->twin = 0;
}

ProgramFunction::ProgramFunction() {
    this->twin = 0;
}

void ProgramFunction::pair(ProgramFunction *other) {
    this->twin = other;
    other->twin = this;
}

void ProgramFunction::setAnnotationHandler(AnnotationHandler a) {
    this->a = a;
}

AnnotationHandler *ProgramFunction::getAnnotationHandler() {
    return &a;
}

int ProgramFunction::currID() {
    return this->currentAliasNum;
}

void ProgramFunction::addProgramBlock(ProgramBlock programBlock) {
    programBlock.parent = this;
    this->programBlocks.push_back(programBlock);
}

int ProgramFunction::getNewID() {
    if(this->twin) {
        this->currentAliasNum = this->currentAliasNum > this->twin->currentAliasNum ? this->currentAliasNum : this->twin->currentAliasNum;
        this->twin->currentAliasNum = this->currentAliasNum > this->twin->currentAliasNum ? this->currentAliasNum : this->twin->currentAliasNum;
    }
    this->currentAliasNum += 1;
    if(this->twin) {
        this->currentAliasNum = this->currentAliasNum > this->twin->currentAliasNum ? this->currentAliasNum : this->twin->currentAliasNum;
        this->twin->currentAliasNum = this->currentAliasNum > this->twin->currentAliasNum ? this->currentAliasNum : this->twin->currentAliasNum;
    }
    return this->currentAliasNum - 1;
}

void ProgramFunction::resetID() {
    this->currentAliasNum = 0;
}

std::list<ProgramBlock> ProgramFunction::getProgramBlocks() const {
    return this->programBlocks;
}

ProgramFunction *ProgramFunction::deepCopy() {
    ProgramFunction *newPF = new ProgramFunction(this->getFunctionName());
    newPF->setAnnotationHandler(*this->getAnnotationHandler());
    newPF->currentAliasNum = this->currentAliasNum;
    for(ProgramBlock &b : this->getProgramBlocks()) {
        ProgramBlock *newBlock = newPF->getProgramBlockRef(b.getBlockName(), true);
        newBlock->parent = newPF;
        for(ProgramPoint *P : b.getPoints()) {
            ProgramPoint *p = new ProgramPoint(P->getPointLine(), P);
            p->setParentFunc(newPF);
            newBlock->add(p);
        }
    }
    return newPF;
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
        newProgramBlock.parent = this;
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
        newProgramBlock.parent = this;
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

bool ProgramFunction::checkFixed() {
    bool fixed = true;
    for(auto block : this->getProgramBlocks()) {
        fixed = fixed && block.fixed;
        if(!fixed) {
            break;
        }
    }
    return fixed;
}

void ProgramFunction::logoutProgramFunction(ProgramFunction &programFunction,
        bool logMethods) {
    for (auto block : programFunction.getProgramBlocks()) {
        logout("\n**point name " << block.getBlockName());
        ProgramBlock::logoutProgramBlock(block);
    }
}
