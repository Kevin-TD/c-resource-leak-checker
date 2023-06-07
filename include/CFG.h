#ifndef CFG_H
#define CFG_H

#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "loadFunctions.h"

#include <list> 
#include <set>
#include <map>
#include <set>

class CFG {
private:
    void checkFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, std::string& setCFG); 
    void getFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, CFG*& foundCFG); 
    void traverse(CFG* cfg, std::map<CFG*, bool> cfgTracker); 
    std::string branchName;
    std::set<CFG*> successors; 
    std::set<CFG*> predecessors; 
    std::set<Instruction*> instructions; 
    
public: 
    CFG();
    CFG(std::string branchName, std::set<Instruction*> instructions); 
    CFG(std::string branchName); 
    
    bool checkFind(const std::string& x); 
    CFG* getFind(const std::string& x); 

    void addSuccessor(std::string branchName,  std::set<Instruction*> instructions); 
    void addSuccessor(std::string branchName); 
    void addSuccessor(CFG* cfg); 

    void addPredecessor(std::string branchName,  std::set<Instruction*> instructions); 
    void addPredecessor(std::string branchName); 
    void addPredecessor(CFG* cfg); 

    void setInstructions(std::set<Instruction*> instructions); 

    std::string getBranchName(); 
    std::set<CFG*> getSuccessors(); 
    std::set<CFG*> getPredecessors(); 

    void printInsts(); 

    void traverse(); 

};

#endif
