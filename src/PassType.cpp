#include "PassType.h"
#include "Debug.h"
#include <fstream>

void PassType::setFunctions(std::set<std::string> safeFunctions, std::set<std::string> unsafeFunctions, std::set<std::string> reallocFunctions, std::map<std::string, std::string> memoryFunctions) {
    this->safeFunctions = safeFunctions;
    this->unsafeFunctions = unsafeFunctions; 
    this->reallocFunctions = reallocFunctions; 
    this->memoryFunctions = memoryFunctions; 
}

void PassType::buildExpectedResult(std::string testName) {
  std::ifstream testFile("../test/" + testName + ".txt");
  std::string line; 

  if (testFile.is_open()) {
    while (std::getline(testFile, line)) {
      if (line.size() > 14 && line.substr(0, 14) == "ALLOW_REDEFINE") {
        continue; 
      }

      int spaceCount = 0; 
      std::string branchName;
      std::string varName;
      std::set<std::string> calledMethodsSet; 
      std::string currentCM; 
      std::string passType; 

      for (char c : line) {
        if (c == ' ') {
          spaceCount++;
          continue; 
        }

        if (spaceCount == 0) {
          passType += c; 
        }
        else if (spaceCount == 1) {
          if (passType != this->passName) {
            break; 
          }
          
          branchName += c; 
        }
        else if (spaceCount == 2) {
          varName += c; 
        }
        else if (spaceCount == 3) {
          if (c == '{') {
            continue; 
          }
          if (c == ',' || c == '}') {
            if (currentCM.size() > 0)
              calledMethodsSet.insert(currentCM);
            currentCM = ""; 
            continue; 
          }
          currentCM += c; 


        }
      }

      if (passType == this->passName) {
        this->expectedResult[branchName][varName] = {
          calledMethodsSet, 
          true 
        }; 
      }
    }

  }
  testFile.close(); 
}

MappedMethods PassType::generatePassResults() {
    MappedMethods PreMappedMethods; 
    MappedMethods PostMappedMethods; 
    this->analyzeCFG(this->cfg, PreMappedMethods, PostMappedMethods, ""); 
    return PostMappedMethods; 
}

void PassType::setCFG(CFG* cfg) {
    this->cfg = cfg; 
}

void PassType::setAliasedVars(AliasMap aliasedVars) {
    this->aliasedVars = aliasedVars; 
}

bool getAllowedRedefine(std::string testName) {
    std::ifstream testFile("../test/" + testName + ".txt");
    std::string line; 
    bool ALLOW_REDEFINE;

    if (testFile.is_open()) {
        while (std::getline(testFile, line)) {
            if (line.size() > 14 && line.substr(0, 14) == "ALLOW_REDEFINE") {
                std::string allowedRedefValue = line.substr(15);
                if (allowedRedefValue == "true") {
                    ALLOW_REDEFINE = true; 
                    break;
                }
                else if (allowedRedefValue == "false") {
                    ALLOW_REDEFINE = false; 
                    break; 
                }
            }
        }

    }
    testFile.close(); 

    return ALLOW_REDEFINE; 
}


void PassType::transfer(Instruction* instruction, SetVector<Instruction *> workSet, std::map<std::string, MaybeUninitMethodsSet>& inputMethodsSet) {
   if (auto Alloca = dyn_cast<AllocaInst>(instruction)) {
    /*
    Typically allocation instructions are used for local identifiers which start with a %, so it's fine to add a % to the name here 
    */
    inputMethodsSet["%" + Alloca->getName().str()] = {};
    
  }
  else if (auto Store = dyn_cast<StoreInst>(instruction)) {

    Value* valueToStore = Store->getOperand(0);      
    Value* receivingValue = Store->getOperand(1);   
 
    for (auto Inst : workSet) {
      if (valueToStore == Inst) {
        if (auto Call = dyn_cast<CallInst>(Inst)) {
          std::string argName = dataflow::variable(Store->getOperand(1)); 

          if (argName[0] == '@') {
            argName[0] = '%'; 
          }

          std::string fnName = Call->getCalledFunction()->getName().str(); 


          if (
            !this->unsafeFunctions.count(fnName)  && 
            !this->unsafeFunctions.count(this->memoryFunctions[fnName]) && 
            this->memoryFunctions[fnName].size() > 0
          ) {
            this->onAllocationFunctionCall(inputMethodsSet[argName], fnName); 
          }
          return; 
        }

      }
    }
    
    // for pointer aliasing
    std::string lval = dataflow::variable(receivingValue); 
    std::string rval = dataflow::variable(valueToStore);  

    /*
    LLVM identifiers either start with @ (global identifier) or % (local identifier)
    Docs: https://llvm.org/docs/LangRef.html#identifiers 
    For (temporary decision) simplicity sake, @'s will be replaced with %, though in the future the variable naming system may entirely change to remove %'s and @'s 

    */
    if (lval[0] == '@') {
      lval[0] = '%'; 
    }

    if (rval[0] == '@') {
      rval[0] = '%'; 
    }

    if (lval[0] == rval[0] && lval[0] == '%') {
      aliasedVars[lval] = rval; 
    }
  

  
  }
  else if (auto Call = dyn_cast<CallInst>(instruction)) {
    for (unsigned i = 0; i < Call->getNumArgOperands(); ++i) {
        Value *argument = Call->getArgOperand(i);
        std::string argName = dataflow::variable(argument);

        argName = aliasedVars[argName];
        while (argName.size() > 1 && dataflow::isNumber(argName.substr(1))) {
          argName = aliasedVars[argName]; 
        }


        // * i THINK this is what distinguishes "real" variables defined in the program from constants and other irrelevant llvm stuff 
        if (argName[0] != '%') return; 

        std::list<std::string> allAliases;

        while (argName != "") {
          allAliases.push_back(argName); 
          argName = aliasedVars[argName]; 
        }

        for (auto alias : allAliases) {
          logout("alias = " << alias)
        }


        // * (i think this handles) cases where an undefined to us function shows up 
        if (Call->getCalledFunction() == NULL) {
          const DebugLoc &debugLoc = instruction->getDebugLoc();
          std::string location = "Line " + std::to_string(debugLoc.getLine()) + ", Col " + std::to_string(debugLoc.getCol());

          errs() << "WARNING: unknown (unforseen occurence) & unsafe func on " << location << "\n";

          for (std::string aliasArg : allAliases) {
            this->onUnknownFunctionCall(inputMethodsSet[aliasArg]); 
          }
          
          return; 
        }

        std::string fnName = Call->getCalledFunction()->getName().str();


        if (this->unsafeFunctions.count(fnName)) {
          for (std::string aliasArg : allAliases) {
            this->onUnsafeFunctionCall(inputMethodsSet[aliasArg], fnName); 
          }
          return; 
        }

        if (this->reallocFunctions.count(fnName)) {
          for (std::string aliasArg : allAliases) {
            this->onReallocFunctionCall(inputMethodsSet[aliasArg], fnName); 
          }
          return; 
        }

        if (this->safeFunctions.count(fnName)) {
          for (std::string aliasArg : allAliases) {
            this->onSafeFunctionCall(inputMethodsSet[aliasArg], fnName); 
          }
          return; 
        }


        for (auto Pair : this->memoryFunctions) {
          if (fnName == Pair.first) {
            for (std::string aliasArg : allAliases) {
              this->onAllocationFunctionCall(inputMethodsSet[aliasArg], fnName);                 
            }
            break; 
          }
          else if (fnName == Pair.second) { 
            for (std::string aliasArg : allAliases) {
              this->onDeallocationFunctionCall(inputMethodsSet[aliasArg], fnName);                 
            }
            break; 
          }
        }
    }
  }
}


void PassType::analyzeCFG(CFG* cfg, MappedMethods& PreMappedMethods, MappedMethods& PostMappedMethods, std::string priorBranch) {
  std::string currentBranch =  cfg->getBranchName(); 

  if (currentBranch == "entry") { 

    PreMappedMethods[currentBranch] = {}; 
    llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 

    for (Instruction* instruction : instructions) {
      transfer(instruction, instructions, PostMappedMethods[currentBranch]);
    }


    for (CFG* succ : cfg->getSuccessors()) {
      analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch); 
    }

  }

  else {
    std::map<std::string, MaybeUninitMethodsSet> PriorPreMC = std::map<std::string, MaybeUninitMethodsSet>(PreMappedMethods[currentBranch]); 
    std::map<std::string, MaybeUninitMethodsSet> PriorPostMC = std::map<std::string, MaybeUninitMethodsSet>(PostMappedMethods[currentBranch]); 


    if (PriorPreMC.size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch)

      std::map<std::string, MaybeUninitMethodsSet> CurrentPreMC = std::map<std::string, MaybeUninitMethodsSet>(PostMappedMethods[priorBranch]);
      

      // check if inputs (pre) differ
      bool currentAndPriorPreMCEqual = true; 
      for (auto Pair : CurrentPreMC) {
        std::string varName = Pair.first; 
        MaybeUninitMethodsSet currentPreMC = Pair.second; 

        if (
          (currentPreMC.methodsSet != PriorPreMC[varName].methodsSet) || 
          (currentPreMC.setInitialized != PriorPreMC[varName].setInitialized)
        ) {
          currentAndPriorPreMCEqual = false; 
          break; 
        }
      }

      for (auto Pair : PriorPreMC) {
        std::string varName = Pair.first; 
        MaybeUninitMethodsSet priorPreMC = Pair.second; 

        if (
          (priorPreMC.methodsSet != CurrentPreMC[varName].methodsSet) || 
          (priorPreMC.setInitialized != CurrentPreMC[varName].setInitialized)
        ) {
          currentAndPriorPreMCEqual = false; 
          break; 
        }

      }

      if (currentAndPriorPreMCEqual) return; 
    
      // lub PriorPreCM and CurrentPreCM
      std::map<std::string, MaybeUninitMethodsSet> lub; 

      for (auto Pair1 : PriorPreMC) {
        std::string varName = Pair1.first; 
        MaybeUninitMethodsSet priorPreMC = Pair1.second; 
        
        std::set<std::string> setUnion;

        this->leastUpperBound(priorPreMC, CurrentPreMC[varName], setUnion); 

        lub[varName] = {
          setUnion, 
          true
        }; 
        
      
      }

    
      // fill in the rest 
      for (auto Pair1 : PriorPostMC) {
        std::string varName = Pair1.first; 
        MaybeUninitMethodsSet priorPostMC = Pair1.second; 

        if (!lub[varName].setInitialized) {
          
          lub[varName] = {
            std::set<std::string>(priorPostMC.methodsSet), 
            true 
          }; 

        }
      }

      PreMappedMethods[currentBranch] = std::map<std::string, MaybeUninitMethodsSet>(lub);

      llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 
      for (Instruction* instruction : instructions) {
        transfer(instruction, instructions, lub);
      }

      PostMappedMethods[currentBranch] = lub; 

      for (CFG* succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch); 
      }


    }
    else {
      logout("doing normal flow for " << currentBranch << " and prior = " << priorBranch)  

      std::map<std::string, MaybeUninitMethodsSet> priorPostCM = std::map<std::string, MaybeUninitMethodsSet>(PostMappedMethods[priorBranch]); 

      PreMappedMethods[currentBranch] = std::map<std::string, MaybeUninitMethodsSet>(priorPostCM); 
      llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 

      std::map<std::string, MaybeUninitMethodsSet> flowInto = std::map<std::string, MaybeUninitMethodsSet>(PostMappedMethods[priorBranch]); 

       for (Instruction* instruction : instructions) {
        transfer(instruction, instructions, flowInto);
      }

      PostMappedMethods[currentBranch] = flowInto;

      for (CFG* succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch); 
      }

    }

  }
}

MappedMethods PassType::getExpectedResult() {
  return this->expectedResult; 
}