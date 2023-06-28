#include "MustCall.h"

#define DEBUG true
#if DEBUG
#define logout(x) errs() << x << "\n";
#define logDomain(x) x->print(errs()); 
#define logOutMemory(x) printMemory(x);
#else
#define logout(x) 
#define logDomain(x) 
#define logOutMemory(x) 
#endif 

MustCall::MustCall(std::string passName) {
    this->passName = passName; 
}

void MustCall::transfer(Instruction* I, SetVector<Instruction *> workSet, std::map<std::string, MethodsSetHolder>& inputMethodsSet) {
   if (auto Alloca = dyn_cast<AllocaInst>(I)) {
    inputMethodsSet["%" + Alloca->getName().str()] = {};
    
  }
  else if (auto Load = dyn_cast<LoadInst>(I)) {
    logout("(load) name is " << dataflow::variable(Load) << " for " << dataflow::variable(Load->getPointerOperand()) )

      std::string varName = dataflow::variable(Load->getPointerOperand()); 
      while (varName.size() > 1 && dataflow::isNumber(varName.substr(1))) {
        varName = aliasedVars[varName]; 
      }

      logout(dataflow::variable(Load) << " -> " << varName)

      aliasedVars[dataflow::variable(Load)] = varName;
  }
  else if (auto Store = dyn_cast<StoreInst>(I)) {

    Value* valueToStore = Store->getOperand(0);      
    Value* receivingValue = Store->getOperand(1);   
 
    for (auto Inst : workSet) {
      if (valueToStore == Inst) {
        if (auto Call = dyn_cast<CallInst>(Inst)) {
          std::string argName = dataflow::variable(Store->getOperand(1)); 
          std::string fnName = Call->getCalledFunction()->getName().str(); 


          if (
            !this->unsafeFunctions[fnName]  && 
            !this->unsafeFunctions[this->memoryFunctions[fnName]] && 
            this->memoryFunctions[fnName].size() > 0
          ) {
            //  calledMethodsEst[branchName][varName].insert(fnName);   
            //  allocation function 
            inputMethodsSet[argName].methodsSet.insert(fnName);
            inputMethodsSet[argName].setInitialized = true; 
          }
          return; 
        }

      }
    }
    
    // for pointer aliasing
    std::string lval = dataflow::variable(receivingValue); 
    std::string rval = dataflow::variable(valueToStore);  

    if (lval[0] == rval[0] && lval[0] == '%') {
      aliasedVars[lval] = rval; 
    }
  

  
  }
  else if (auto Call = dyn_cast<CallInst>(I)) {
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
        allAliases.push_back(argName); 
        while (argName != "") {
          argName = aliasedVars[argName]; 
          if (argName != "") 
            allAliases.push_back(argName); 
        }

        for (auto alias : allAliases) {
          logout("alias = " << alias)
        }


        // * (i think this handles) cases where an entirely random and undefined (implicitly declared) function shows up 
        if (Call->getCalledFunction() == NULL) {
          const DebugLoc &debugLoc = I->getDebugLoc();
          std::string location = "Line " + std::to_string(debugLoc.getLine()) + ", Col " + std::to_string(debugLoc.getCol());

          errs() << "MC-WARNING: unknown (unforseen occurence) & unsafe func on " << location << ". call methods set for '" << argName << "' {nothing changes}.\n";

        //   for (std::string aliasArg : allAliases) {
        //     inputMethodsSet[aliasArg].methodsSet = {}; 
        //   }
          
          return; 
        }

        std::string fnName = Call->getCalledFunction()->getName().str();


        if (this->unsafeFunctions[fnName]) {
          logout("unsafe func, mc (?)")
          for (std::string aliasArg : allAliases) {
            // inputMethodsSet[aliasArg].cmSet = {}; 
          }

          return; 
        }

        if (this->reallocFunctions[fnName]) {
          logout("realloc func, mc does nothing")
          return; 
        }

        if (this->safeFunctions[fnName]) {
          logout("safe func")
          return; 
        }


        for (auto Pair : this->memoryFunctions) {
          if (fnName == Pair.first) { // allocation function
            for (std::string aliasArg : allAliases) {
                logout("INSERTX " << fnName)
                inputMethodsSet[aliasArg].methodsSet.insert(fnName); 
                inputMethodsSet[aliasArg].setInitialized = true; 
            }
          }
          else if (fnName == Pair.second) { // deallocation function 
            
            break; 
          }
        }
        



    }
  }
}


void MustCall::analyzeCFG(CFG* cfg, MappedMethods& PreMappedMethods, MappedMethods& PostMappedMethods, std::string priorBranch) {
  std::string currentBranch =  cfg->getBranchName(); 

  if (currentBranch == "entry") { 

    PreMappedMethods[currentBranch] = {}; 
    llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 

    for (Instruction* I : instructions) {
      transfer(I, instructions, PostMappedMethods[currentBranch]);
    }


    for (CFG* succ : cfg->getSuccessors()) {
      analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch); 
    }

  }

  else {
    std::map<std::string, MethodsSetHolder> PriorPreMC = std::map<std::string, MethodsSetHolder>(PreMappedMethods[currentBranch]); 
    std::map<std::string, MethodsSetHolder> PriorPostMC = std::map<std::string, MethodsSetHolder>(PostMappedMethods[currentBranch]); 


    logout("-----\nlogging priorprecm prior = " << priorBranch << " current = " << currentBranch )
    for (auto p : PriorPreMC) {
      std::string m; 
      for (auto k : p.second.methodsSet) {
        m += k + ", ";
      }
      logout(p.first << " " << m)
    }


    if (PriorPreMC.size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch)

      std::map<std::string, MethodsSetHolder> CurrentPreMC = std::map<std::string, MethodsSetHolder>(PostMappedMethods[priorBranch]);
      

      // check if inputs (pre) differ
      bool currentAndPriorPreMCEqual = true; 
      for (auto Pair : CurrentPreMC) {
        std::string varName = Pair.first; 
        MethodsSetHolder currentPreMC = Pair.second; 

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
        MethodsSetHolder priorPreMC = Pair.second; 

        if (
          (priorPreMC.methodsSet != CurrentPreMC[varName].methodsSet) || 
          (priorPreMC.setInitialized != CurrentPreMC[varName].setInitialized)
        ) {
          currentAndPriorPreMCEqual = false; 
          break; 
        }

      }

      if (currentAndPriorPreMCEqual) return; 

      llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 
    
      // lub PriorPreCM and CurrentPreCM
      std::map<std::string, MethodsSetHolder> lub; 

      for (auto Pair1 : PriorPreMC) {
        std::string varName = Pair1.first; 
        MethodsSetHolder priorPreMC = Pair1.second; 
        
          

        std::set<std::string> setUnion;

        std::set_union(priorPreMC.methodsSet.begin(), priorPreMC.methodsSet.end(), 
                      CurrentPreMC[varName].methodsSet.begin(), CurrentPreMC[varName].methodsSet.end(), 
                      std::inserter(setUnion, setUnion.begin()));

        logout("for var name " << varName)

        std::string m,k; 
        for (auto i : priorPreMC.methodsSet) {
          m += i + ", ";
        }
        for (auto i : CurrentPreMC[varName].methodsSet) {
          k += i + ", ";
        }
        logout("prior pre mc = " << m)
        logout("current pre mc = " << k)


        lub[varName] = {
          setUnion, 
          true
        }; 
        
      
      }

    
      // fill in the rest 
      for (auto Pair1 : PriorPostMC) {
        std::string varName = Pair1.first; 
        MethodsSetHolder priorPostMC = Pair1.second; 

        if (!lub[varName].setInitialized) {
          
          lub[varName] = {
            std::set<std::string>(priorPostMC.methodsSet), 
            true 
          }; 

        }
      }

      PreMappedMethods[currentBranch] = std::map<std::string, MethodsSetHolder>(lub);

      for (Instruction* I : instructions) {
        transfer(I, instructions, lub);
      }

      PostMappedMethods[currentBranch] = lub; 

      for (CFG* succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch); 
      }


    }
    else {
      logout("doing normal flow for " << currentBranch << " and prior = " << priorBranch)  

      std::map<std::string, MethodsSetHolder> priorPostCM = std::map<std::string, MethodsSetHolder>(PostMappedMethods[priorBranch]); 

      PreMappedMethods[currentBranch] = std::map<std::string, MethodsSetHolder>(priorPostCM); 
      llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 

      std::map<std::string, MethodsSetHolder> flowInto = std::map<std::string, MethodsSetHolder>(PostMappedMethods[priorBranch]); 

       for (Instruction* I : instructions) {
        transfer(I, instructions, flowInto);
      }

      PostMappedMethods[currentBranch] = flowInto;

      

      for (CFG* succ : cfg->getSuccessors()) {
        analyzeCFG(succ, PreMappedMethods, PostMappedMethods, currentBranch); 
      }

    }

  }
}

