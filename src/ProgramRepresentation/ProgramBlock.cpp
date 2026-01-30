#include "ProgramRepresentation/ProgramBlock.h"
#include "ProgramRepresentation/ProgramPoint.h"
#include "ProgramRepresentation/ProgramVariable.h"
#include "Debug.h"

ProgramBlock::ProgramBlock(std::string blockName) {
    this->blockName = blockName;
}

ProgramBlock::ProgramBlock() {}

ProgramBlock::ProgramBlock(std::string blockName, ProgramBlock *prev) {
    this->blockName = blockName;
    this->points.push_back(prev->getPoints().back());
}

std::list<ProgramPoint *> ProgramBlock::getPoints() {
    return this->points;
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

void ProgramBlock::update(int point) {
    ProgramPoint *one, *two;
    one = this->getPoint(point-1, false);
    two = this->getPoint(point, true);
    ProgramVariable oneVar;
    PVAliasSet *oneSet, *twoSet;
    int run = 0;
    if(one != two) {
        for(auto set : one->getProgramVariableAliasSets().getSets()) {
            twoSet = two->getSetID(set.getID());
            oneSet = one->getSetID(set.getID());
            // This is run intra blocks, therefore a union here is safe. Between blocks will need to be an intersection between successors
            if(!oneSet)
                continue;
            else if(!twoSet) {
                two->addPVAS(*oneSet);
            } else {
                twoSet->methodsSetUnion(oneSet->getMethodsSet());
            }
        }
    }
}

ProgramPoint *ProgramBlock::getPoint(unsigned int line, bool addNew) {
    ProgramPoint *last = this->points.front();
    this->points.sort([](ProgramPoint *a, ProgramPoint *b) {
        return a->getPointLine() < b->getPointLine();
    });

    for(auto p : this->points) {
        if(p->getPointLine() == line) {
            return p;
        }
        if(p->getPointLine() < line)
            last = p;
    }
    if(!addNew)
        return NULL;
    // Create new point
    ProgramPoint *newP;
    if(last && last->getPointLine() < line) {
        llvm::errs() << "Creating " << line << " from " << last->getPointLine() << "\n";
        newP = new ProgramPoint(line, last);
    } else {
        llvm::errs() << "Creating completely new in " << this << "\n";
        newP = new ProgramPoint(line);
    }
    this->points.push_back(newP);
    this->points.sort([](ProgramPoint *a, ProgramPoint *b) {
        return a->getPointLine() < b->getPointLine();
    });
    return newP;
}

void ProgramBlock::addSuccessor(ProgramBlock *p) {
    this->successors.push_back(p);
}

std::list<ProgramBlock *> ProgramBlock::getSuccessors() {
    return successors;
}

