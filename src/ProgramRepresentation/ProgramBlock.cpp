#include "ProgramRepresentation/ProgramBlock.h"
#include "ProgramRepresentation/ProgramPoint.h"
#include "ProgramRepresentation/ProgramVariable.h"
#include "Debug.h"

ProgramBlock::ProgramBlock(std::string blockName) {
    this->blockName = blockName;
}

ProgramBlock::ProgramBlock() {}

ProgramBlock::ProgramBlock(std::string pointName, ProgramBlock *prev) {
    this->blockName = blockName;
    this->points.push_back(&prev->getPoints().back());
}

void ProgramBlock::logoutProgramBlock(const ProgramBlock &block) {
    logout("\n**point name " << block.getBlockName());
}

void ProgramBlock::logoutProgramBlock(const ProgramBlock *block) {
    logout("\n**point name " << block->getBlockName());
}

std::string ProgramBlock::getBlockName() const {
    return this->blockName;
}

bool ProgramBlock::varExists(ProgramVariable programVar) {
    bool ret = false;
    for(auto a : this->points) {
        ret = a->varExists(programVar);
        if(ret)
            return true;
    }
    return false;
}

Value *ProgramBlock::getReturnValue() {
    return returnValue;
}

void ProgramBlock::add(ProgramPoint *programPoint) {
    this->points.push_back(programPoint);
}

ProgramPoint *ProgramBlock::getPoint(unsigned int line) {
    if(line > this->points.back()->getPointName()) {
        return this->points.back();
    }
    ProgramPoint *last = this->points.front();
    // I don't know if this part is necessary if everything in a program point is continuous but it
    // is here just in case
    for(auto p : this->points) {
        if(p->getPointName() > line) {
            return last;
        }
        last = p;
    }
    return last;
}

void ProgramBlock::addSuccessor(ProgramBlock *p) {
    this->successors.push_back(p);
}

std::list<ProgramBlock *> ProgramBlock::getSuccessors() {
    return successors;
}

