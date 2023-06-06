#ifndef CFG_H
#define CFG_H

#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "loadFunctions.h"

#include <list> 
#include <map>
#include <set>

class CFG {
private:
    void checkFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, std::string& setCFG); 
    void getFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, CFG*& foundCFG); 
    
public: 
    std::string branchName;
    std::list<CFG*> successors; 
    std::list<CFG*> predecessors; 
    std::list<Instruction*> instructions; 

    CFG();
    CFG(std::string branchName, std::list<Instruction*> instructions); 
    CFG(std::string branchName); 
    
    bool checkFind(const std::string& x); 
    CFG* getFind(const std::string& x); 

    void addSuccessor(std::string branchName,  std::list<Instruction*> instructions); 
    void addSuccessor(std::string branchName); 
    void addSuccessor(CFG* cfg); 

    void addPredecessor(std::string branchName,  std::list<Instruction*> instructions); 
    void addPredecessor(std::string branchName); 
    void addPredecessor(CFG* cfg); 

    std::string getBranchName(); 
    void printInsts(); 

    bool successorIsDefined(std::string x, CFG* cfg); 
    bool predecessorIsDefined(std::string x, CFG* cfg); 

    std::string getAllSuccsString(std::set<std::string> v, CFG* cfg);
    std::string getAllPredsString(std::set<std::string> v, CFG* cfg);

};

#endif
