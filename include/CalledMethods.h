#ifndef CALLED_METHODS_H
#define CALLED_METHODS_H

#include "PassType.h"

class CalledMethods : public PassType {
private:
    AliasMap aliasedVars; 
    std::string testName; 
    CFG* cfg; 
    MappedMethods expectedResult; 
    
    void analyzeCFG(CFG* cfg, MappedMethods& PreMappedMethods, MappedMethods& PostMappedMethods, std::string priorBranch); 
    void transfer(Instruction* I,  SetVector<Instruction *> workSet, std::map<std::string, MethodsSetHolder>& inputMethodsSet);

public: 
    CalledMethods(std::string passName); 
};


#endif 