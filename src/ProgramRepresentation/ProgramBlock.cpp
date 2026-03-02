#include "ProgramRepresentation/ProgramBlock.h"
#include "ProgramRepresentation/ProgramPoint.h"
#include "ProgramRepresentation/ProgramVariable.h"
#include "Debug.h"

ProgramBlock::ProgramBlock(std::string blockName) {
    this->blockName = blockName;
    this->fixed = false;
}

ProgramBlock::ProgramBlock() {
    this->fixed = false;
}

ProgramBlock::ProgramBlock(std::string blockName, ProgramBlock *prev) {
    this->blockName = blockName;
    this->parent = prev->parent;
    this->points.push_back(prev->getPoints().back());
    this->fixed = false;
}

std::list<ProgramPoint *> ProgramBlock::getPoints() {
    return this->points;
}

void ProgramBlock::logoutProgramBlock(const ProgramBlock &block) {
    logout("\n**block name " << block.getBlockName());
    for(auto point : block.points) {
        ProgramPoint::logoutProgramPoint(point, true);
        llvm::errs() << "-----\n";
    }
}

void ProgramBlock::logoutProgramBlock(const ProgramBlock *block) {
    logout("\n**block name " << block->getBlockName());
    for(auto point : block->points) {
        ProgramPoint::logoutProgramPoint(point, true);
        llvm::errs() << "-----\n";
    }
}

PVAliasSet *ProgramBlock::getPVASRefFromValue(Value* value) {
    for (ProgramPoint* programPoint : this->points) {
        if (PVAliasSet* pvas = programPoint->getPVASRef(value, false)) {
            return pvas;
        }
    }

    return NULL;
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
    programPoint->setParentFunc(this->parent);
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
ProgramPoint *ProgramBlock::getLast() {
    return this->points.back();
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
        llvm::errs() << "Creating completely new in " << this << " name: " << this->blockName<< " Parent " << this->parent <<  "\n";
        newP = new ProgramPoint(line);
    }
    newP->setParentFunc(this->parent);
    this->points.push_back(newP);
    this->points.sort([](ProgramPoint *a, ProgramPoint *b) {
        return a->getPointLine() < b->getPointLine();
    });
    return newP;
}

void ProgramBlock::addSuccessor(ProgramBlock *p) {
    auto succ = this->successors;
    std::string name = p->getBlockName();
    if(succ.end() == find_if(succ.begin(), succ.end(), [name](const ProgramBlock *b) {
    return b->getBlockName() == name;
    })) {
        this->successors.push_back(p);
    }
}

std::list<ProgramBlock *> ProgramBlock::getSuccessors() {
    return successors;
}

