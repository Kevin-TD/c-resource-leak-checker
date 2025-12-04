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
	ProgramPoint *last = this->points.front();
    for(auto p : this->points) {
        if(p->getPointName() == line) {
            return p;
        }
	if(line < p->getPointName())
		last = p;
    }
    // Create new point
    ProgramPoint *newP;
    if(last)
    	newP = new ProgramPoint(line, last);
    else
	newP = new ProgramPoint(line);
    this->points.push_back(newP);
    // The way we calculate last assumes sorting
    this->points.sort([](const ProgramPoint *P, const ProgramPoint *P2){ return P->getPointName() < P2->getPointName(); });
    return last;
}

void ProgramBlock::addSuccessor(ProgramBlock *p) {
    this->successors.push_back(p);
}

std::list<ProgramBlock *> ProgramBlock::getSuccessors() {
    return successors;
}

