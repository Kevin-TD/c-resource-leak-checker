#ifndef TREE_H
#define TREE_H

#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "loadFunctions.h"

#include <list> 
#include <map>
#include <set>

class Tree {
private:
    void checkFind(const std::string& x, Tree* tree, std::map<Tree*, bool> treeTracker, std::string& setTrue); 
    Tree* getFind(const std::string& x, Tree* tree, std::map<Tree*, bool> treeTracker); 
    
public: 
    std::string branchName;
    std::list<Tree*> successors; 
    std::list<Tree*> predecessors; 
    std::list<Instruction*> instructions; 

    Tree();
    Tree(std::string branchName, std::list<Instruction*> instructions); 
    Tree(std::string branchName); 
    
    bool checkFind(const std::string& x); 
    Tree* getFind(const std::string& x); 

    void addSuccessor(std::string branchName,  std::list<Instruction*> instructions); 
    void addSuccessor(std::string branchName); 
    void addSuccessor(Tree* tree); 

    void addPredecessor(std::string branchName,  std::list<Instruction*> instructions); 
    void addPredecessor(std::string branchName); 

    std::map<int, std::set<std::string>> printTree(Tree* tree, int level, std::map<int, std::set<std::string>> m); 
    std::string getBranchName(); 
    void printInsts(); 

    bool successorIsDefined(std::string x, Tree* tree); 
    bool predecessorIsDefined(std::string x, Tree* tree); 

    std::string getAllSuccsString(std::set<std::string> v, Tree* tree);
    std::string getAllPredsString(std::set<std::string> v, Tree* tree);

};

#endif
