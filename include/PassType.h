#ifndef PASS_H
#define PASS_H

#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "CFG.h"

#include <set>
#include <fstream>

typedef std::map<std::string, std::string> AliasMap;

struct MethodsSetHolder {
    std::set<std::string> methodsSet; 
    bool setInitialized; 
};

typedef std::map<std::string, std::map<std::string, MethodsSetHolder>> MappedMethods;

class PassType {
private: 
    AliasMap aliasedVars; 
    std::string testName; 
    CFG* cfg; 
    MappedMethods expectedResult; 
    
    virtual void analyzeCFG(CFG* cfg, MappedMethods& PreMappedMethods, MappedMethods& PostMappedMethods, std::string priorBranch) = 0; 
    virtual void transfer(Instruction* I,  SetVector<Instruction *> workSet, std::map<std::string, MethodsSetHolder>& inputMethodsSet) = 0;
public:    
    void setFunctions(std::map<std::string, bool> safeFunctions, std::map<std::string, bool> unsafeFunctions, std::map<std::string, bool> reallocFunctions, std::map<std::string, std::string> memoryFunctions); 
    void buildExpectedResult(std::string testName); 

    std::map<std::string, bool> safeFunctions;
    std::map<std::string, bool> unsafeFunctions; 
    std::map<std::string, bool> reallocFunctions;
    std::map<std::string, std::string> memoryFunctions;
    std::string passName; 

   
    bool runTests(MappedMethods receivedResult); 
    MappedMethods generatePassResults(); 

    void setCFG(CFG* cfg); 
    void setAliasedVars(AliasMap aliasedVars); 
}; 


bool getAllowedRedefine(std::string testName); 

#endif 