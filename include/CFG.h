#ifndef CFG_H
#define CFG_H

#include "RunAnalysis.h"
#include "Utils.h"

#include <list> 
#include <set>
#include <map>
#include <set>

class CFG {
private:
    void checkFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, std::string& setCFG); 
    void getFind(const std::string& x, CFG* cfg, std::map<CFG*, bool> cfgTracker, CFG*& foundCFG); 
    std::string branchName;
    std::set<CFG*> successors; 
    std::set<CFG*> predecessors; 
    llvm::SetVector<Instruction*> instructions; 
    
public: 
    CFG();
    CFG(std::string branchName, llvm::SetVector<Instruction*> instructions); 
    CFG(std::string branchName); 
    
    bool checkFind(const std::string& x); 
    CFG* getFind(const std::string& x); 

    void addSuccessor(std::string branchName,  llvm::SetVector<Instruction*> instructions); 
    CFG* addSuccessor(std::string branchName); 
    void addSuccessor(CFG* cfg); 

    void addPredecessor(std::string branchName,  llvm::SetVector<Instruction*> instructions); 
    void addPredecessor(std::string branchName); 
    void addPredecessor(CFG* cfg); 

    void setInstructions(llvm::SetVector<Instruction*> instructions); 

    std::string getBranchName(); 
    std::set<CFG*> getSuccessors(); 
    std::set<CFG*> getPredecessors(); 
    llvm::SetVector<Instruction*> getInstructions(); 

};

#endif
