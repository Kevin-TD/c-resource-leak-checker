#include "CFG.h"

CFG::CFG() {
    this->branchName = "";
}

CFG::CFG(std::string branchName) {
    this->branchName = branchName;
}

CFG::CFG(std::string branchName, std::list<Instruction*> instructions) {
    this->branchName = branchName;
    this->instructions = instructions; 
}


void CFG::addSuccessor(std::string branchName,  std::list<Instruction*> instructions) {
    this->successors.push_back(new CFG(branchName, instructions)); 
}

void CFG::addSuccessor(std::string branchName) {
    this->successors.push_back(new CFG(branchName)); 
}

void CFG::addSuccessor(CFG* tree) {
    this->successors.push_back(tree); 
}


void CFG::addPredecessor(std::string branchName,  std::list<Instruction*> instructions) {
    this->predecessors.push_back(new CFG(branchName, instructions)); 
}

void CFG::addPredecessor(std::string branchName) {
    this->predecessors.push_back(new CFG(branchName)); 
}

void CFG::addPredecessor(CFG* cfg) {
    this->predecessors.push_back(cfg); 
}


std::string CFG::getBranchName() {
    return this->branchName; 
}

CFG* CFG::getFind(const std::string& x)  {
    std::map<CFG*, bool> cfgTracker; 
    CFG* foundCFG = new CFG(); 
    getFind(x, this, cfgTracker, foundCFG); 
    return foundCFG; 
}

void CFG::getFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, CFG*& foundCFG)  {
    if (foundCFG->branchName != "") return; 

    if (x == cfg->branchName) {
        foundCFG = cfg; 
        return; 
    }

    cfgTracker[cfg] = true; 

    for (CFG* succ : cfg->successors) {
        if (x == succ->getBranchName()) {
            foundCFG = succ; 
            return; 
        }
    }
    for (CFG* succ : cfg->successors) {
        if (!cfgTracker[succ]) {
            getFind(x, succ, cfgTracker, foundCFG); 
        }
    }
    

    for (CFG* pred : cfg->predecessors) {
        if (x == pred->getBranchName()) {
            foundCFG = pred; 
            return; 
        }
    }

    for (CFG* pred : cfg->predecessors) {
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

    if (x == cfg->branchName) {
        setTrue = x; 
        return;
    }


    cfgTracker[cfg] = true; 

    for (CFG* succ : cfg->successors) {
        if (x == succ->getBranchName()) {
            setTrue = x; 
            return;
        }
    }
    for (CFG* succ : cfg->successors) {
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

bool CFG::successorIsDefined(std::string x, CFG* cfg) {

    for (CFG* succ : cfg->successors) {
        if (x == succ->getBranchName()) {
            return true; 
        }
    }
    for (CFG* succ : cfg->successors) {
        return successorIsDefined(x, succ); 
    }
    

    return false;
}   

bool CFG::predecessorIsDefined(std::string x, CFG* cfg) {
    for (CFG* pred : cfg->predecessors) {
        if (x == pred->getBranchName()) {
            return true; 
        }
    }

    for (CFG* pred : cfg->predecessors) {
        return predecessorIsDefined(x, pred); 
    }

    return false;
}


std::string CFG::getAllSuccsString(std::set<std::string> v, CFG* cfg) {
    for (CFG* succ : cfg->successors) {
        v.insert(succ->branchName); 
    }

    for (CFG* succ : cfg->successors) {
        getAllSuccsString(v, succ); 
    }

    std::string x; 
    for (auto m : v) {
        x += m + ", "; 
    }
    return x; 

}

std::string CFG::getAllPredsString(std::set<std::string> v, CFG* cfg) {
    for (CFG* succ : cfg->predecessors) {
        v.insert(succ->branchName); 
    }

    for (CFG* succ : cfg->predecessors) {
        getAllPredsString(v, succ); 
    }

    std::string x; 
    for (auto m : v) {
        x += m + ", "; 
    }
    return x; 

}
