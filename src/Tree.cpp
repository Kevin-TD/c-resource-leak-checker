#include "Tree.h"

Tree::Tree() {
    this->branchName = "";
}

Tree::Tree(std::string branchName) {
    this->branchName = branchName;
}

Tree::Tree(std::string branchName, std::list<Instruction*> instructions) {
    this->branchName = branchName;
    this->instructions = instructions; 
}


void Tree::addSuccessor(std::string branchName,  std::list<Instruction*> instructions) {
    this->successors.push_back(new Tree(branchName, instructions)); 
}

void Tree::addSuccessor(std::string branchName) {
    this->successors.push_back(new Tree(branchName)); 
}

void Tree::addSuccessor(Tree* tree) {
    this->successors.push_back(tree); 
}

void Tree::addPredecessor(std::string branchName,  std::list<Instruction*> instructions) {
    this->predecessors.push_back(new Tree(branchName, instructions)); 
}

void Tree::addPredecessor(std::string branchName) {
    this->predecessors.push_back(new Tree(branchName)); 
}

std::string Tree::getBranchName() {
    return this->branchName; 
}

Tree* Tree::getFind(const std::string& x)  {
    std::map<Tree*, bool> treeTracker; 
    return getFind(x, this, treeTracker); 
}

Tree* Tree::getFind(const std::string& x, Tree* tree, std::map<Tree*, bool> treeTracker)  {
    if (x == tree->branchName) {
        return tree; 
    }

    treeTracker[tree] = true; 

    for (Tree* succ : tree->successors) {
        if (x == succ->getBranchName()) {
            return succ; 
        }
    }
    for (Tree* succ : tree->successors) {
        if (!treeTracker[succ]) {
            return getFind(x, succ, treeTracker); 
        }
    }
    

    for (Tree* pred : tree->predecessors) {
        if (x == pred->getBranchName()) {
            return pred; 
        }
    }

    for (Tree* pred : tree->predecessors) {
        if (!treeTracker[pred]) {
            return getFind(x, pred, treeTracker); 
        }
    }

    return new Tree();  // brachName = ""

}

bool Tree::checkFind(const std::string& x) {
    std::map<Tree*, bool> treeTracker; 
    std::string setTrue = ""; 
    checkFind(x, this, treeTracker, setTrue); 
    return setTrue != ""; 
}

void Tree::checkFind(const std::string& x, Tree* tree, std::map<Tree*, bool> treeTracker, std::string& setTrue) {
    errs() << "looking at " << tree->branchName << "\n";
    if (x == tree->branchName) {
        setTrue = x; 
        errs() << "equals main " << x << "\n";
        // return true;
        return;
    }


    treeTracker[tree] = true; 

    for (Tree* succ : tree->successors) {
        if (x == succ->getBranchName()) {
            setTrue = x; 
            errs() << "equals succ " << x << "\n";
            // return true; 
            return;
        }
    }
    for (Tree* succ : tree->successors) {
        if (!treeTracker[succ]) {
            checkFind(x, succ, treeTracker, setTrue); 
        } 
    }
    

    for (Tree* pred : tree->predecessors) {
        if (x == pred->getBranchName()) {
            setTrue = x; 
            errs() << "equals pred " << x << "\n";
            // return true; 
            return; 
        }
    }

    for (Tree* pred : tree->predecessors) {
        if (!treeTracker[pred]) {
            checkFind(x, pred, treeTracker, setTrue); 
        } 
    }

    // return false;
}

void Tree::printInsts() {
    for (auto I : this->instructions) {
        errs() << *I << "\n";
    }
}

std::map<int, std::set<std::string>> Tree::printTree(Tree* tree, int level, std::map<int, std::set<std::string>> m)  {

    m[level].insert(tree->branchName); 

    for (Tree* succ : tree->successors) {
        m[level + 1].insert(succ->branchName);
    }

    for (Tree* pred : tree->predecessors) {
        m[level - 1].insert(pred->branchName);
    }

    for (Tree* succ : tree->successors) {
        return printTree(succ, level + 1, m); 
    }
    

    for (Tree* pred : tree->predecessors) {
        return printTree(pred, level - 1, m); 
    }

    return m; 


}

bool Tree::successorIsDefined(std::string x, Tree* tree) {

    for (Tree* succ : tree->successors) {
        if (x == succ->getBranchName()) {
            return true; 
        }
    }
    for (Tree* succ : tree->successors) {
        return successorIsDefined(x, succ); 
    }
    

    return false;
}   

bool Tree::predecessorIsDefined(std::string x, Tree* tree) {
    for (Tree* pred : tree->predecessors) {
        if (x == pred->getBranchName()) {
            return true; 
        }
    }

    for (Tree* pred : tree->predecessors) {
        return predecessorIsDefined(x, pred); 
    }

    return false;
}


std::string Tree::getAllSuccsString(std::set<std::string> v, Tree* tree) {
    for (Tree* succ : tree->successors) {
        v.insert(succ->branchName); 
    }

    for (Tree* succ : tree->successors) {
        getAllSuccsString(v, succ); 
    }

    std::string x; 
    for (auto m : v) {
        x += m + ", "; 
    }
    return x; 

}

std::string Tree::getAllPredsString(std::set<std::string> v, Tree* tree) {
    for (Tree* succ : tree->predecessors) {
        v.insert(succ->branchName); 
    }

    for (Tree* succ : tree->predecessors) {
        getAllPredsString(v, succ); 
    }

    std::string x; 
    for (auto m : v) {
        x += m + ", "; 
    }
    return x; 

}
