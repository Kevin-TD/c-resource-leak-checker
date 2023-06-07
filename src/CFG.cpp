#include "CFG.h"

CFG::CFG() {
    this->branchName = "";
}

CFG::CFG(std::string branchName) {
    this->branchName = branchName;
}

CFG::CFG(std::string branchName, std::set<Instruction*> instructions) {
    this->branchName = branchName;
    this->instructions = instructions; 
}


void CFG::addSuccessor(std::string branchName,  std::set<Instruction*> instructions) {
    this->successors.insert(new CFG(branchName, instructions)); 
}

void CFG::addSuccessor(std::string branchName) {
    this->successors.insert(new CFG(branchName)); 
}

void CFG::addSuccessor(CFG* tree) {
    this->successors.insert(tree); 
}


void CFG::addPredecessor(std::string branchName,  std::set<Instruction*> instructions) {
    this->predecessors.insert(new CFG(branchName, instructions)); 
}

void CFG::addPredecessor(std::string branchName) {
    this->predecessors.insert(new CFG(branchName)); 
}

void CFG::addPredecessor(CFG* cfg) {
    this->predecessors.insert(cfg); 
}


std::string CFG::getBranchName() {
    return this->branchName; 
}

std::set<CFG*> CFG::getSuccessors() {
    return this->successors; 
}

std::set<CFG*> CFG::getPredecessors() {
    return this->predecessors; 
}

void CFG::setInstructions(std::set<Instruction*> instructions) {
    this->instructions = instructions; 
}

CFG* CFG::getFind(const std::string& x)  {
    std::map<CFG*, bool> cfgTracker; 
    CFG* foundCFG = new CFG(); 
    getFind(x, this, cfgTracker, foundCFG); 
    return foundCFG; 
}

void CFG::getFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, CFG*& foundCFG)  {
    if (foundCFG->getBranchName() != "") return; 

    if (x == cfg->getBranchName()) {
        foundCFG = cfg; 
        return; 
    }

    cfgTracker[cfg] = true; 

    for (CFG* succ : cfg->getSuccessors()) {
        if (x == succ->getBranchName()) {
            foundCFG = succ; 
            return; 
        }
    }
    for (CFG* succ : cfg->getSuccessors()) {
        if (!cfgTracker[succ]) {
            getFind(x, succ, cfgTracker, foundCFG); 
        }
    }
    

    for (CFG* pred : cfg->getPredecessors()) {
        if (x == pred->getBranchName()) {
            foundCFG = pred; 
            return; 
        }
    }

    for (CFG* pred : cfg->getPredecessors()) {
        if (!cfgTracker[pred]) {
            getFind(x, pred, cfgTracker, foundCFG); 
        }
    }

}

bool CFG::checkFind(const std::string& x) {
    std::map<CFG*, bool> cfgTracker; 
    std::string setTrue = ""; 
    checkFind(x, this, cfgTracker, setTrue); 
    return setTrue != ""; 
}

void CFG::checkFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, std::string& setTrue) {
    if (setTrue != "") return; 

    if (x == cfg->getBranchName()) {
        setTrue = x; 
        return;
    }


    cfgTracker[cfg] = true; 

    for (CFG* succ : cfg->getSuccessors()) {
        if (x == succ->getBranchName()) {
            setTrue = x; 
            return;
        }
    }
    for (CFG* succ : cfg->getSuccessors()) {
        if (!cfgTracker[succ]) {
            checkFind(x, succ, cfgTracker, setTrue); 
        } 
    }
    

    for (CFG* pred : cfg->predecessors) {
        if (x == pred->getBranchName()) {
            setTrue = x; 
            return; 
        }
    }

    for (CFG* pred : cfg->predecessors) {
        if (!cfgTracker[pred]) {
            checkFind(x, pred, cfgTracker, setTrue); 
        } 
    }
}

void CFG::printInsts() {
    for (auto I : this->instructions) {
        errs() << *I << "\n";
    }
}

void CFG::traverse() {
    std::map<CFG*, bool> cfgTracker;
    traverse(this, cfgTracker); 
}

void CFG::traverse(CFG* cfg, std::map<CFG*, bool> cfgTracker) {
    errs() << "\nHEAD branch name = " << cfg->getBranchName() << "\n"; 

    cfgTracker[cfg] = true; 

    std::string succs; 
    std::string preds; 

    for (CFG* succ : cfg->getSuccessors()) {
        succs += succ->getBranchName() + ", "; 
    }

    for (CFG* pred : cfg->getPredecessors()) {
        preds += pred->getBranchName() + ", "; 
    }

    errs() << "SUCCS = " << succs << "\n"; 
    errs() << "PREDS = " << preds << "\n\n"; 


    for (CFG* succ : cfg->getSuccessors()) {
        if (!cfgTracker[succ]) {
            traverse(succ, cfgTracker); 
        } 
    }
    

    // for (CFG* pred : cfg->predecessors) {
    //     if (!cfgTracker[pred]) {
    //         traverse(pred, cfgTracker); 
    //     } 
    // }
}
