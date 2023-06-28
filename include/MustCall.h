#ifndef MUST_CALL_H
#define MUST_CALL_H

#include "PassType.h"

class MustCall : public PassType {
private:
    AliasMap aliasedVars; 
    std::string testName; 
    CFG* cfg; 
    MappedMethods expectedResult; 
    
    void analyzeCFG(CFG* cfg, MappedMethods& PreMappedMethods, MappedMethods& PostMappedMethods, std::string priorBranch); 
    void transfer(Instruction* I,  SetVector<Instruction *> workSet, std::map<std::string, MethodsSetHolder>& inputMethodsSet);
public:
    MustCall(std::string passName); 
};


#endif 