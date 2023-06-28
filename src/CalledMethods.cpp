#include "CalledMethods.h"

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


CalledMethods::CalledMethods(std::string passName) {
    this->passName = passName; 
}

void CalledMethods::transfer(Instruction* I, SetVector<Instruction *> workSet, std::map<std::string, MethodsSetHolder>& inputMethodsSet) {
    if (auto Alloca = dyn_cast<AllocaInst>(I)) {
        inputMethodsSet["%" + Alloca->getName().str()] = {};

    }
    else if (auto Load = dyn_cast<LoadInst>(I)) {
        logout("(load) name is " << dataflow::variable(Load) << " for " << dataflow::variable(Load->getPointerOperand()) )

        std::string varName = dataflow::variable(Load->getPointerOperand()); 
        while (varName.size() > 1 && dataflow::isNumber(varName.substr(1))) {
            varName = this->aliasedVars[varName]; 
        }

        logout(dataflow::variable(Load) << " -> " << varName)

        this->aliasedVars[dataflow::variable(Load)] = varName;
    }
    else if (auto Store = dyn_cast<StoreInst>(I)) {

        Value* valueToStore = Store->getOperand(0);      
        Value* receivingValue = Store->getOperand(1);   
    
        for (auto Inst : workSet) {
          if (valueToStore == Inst) {

            // check if it is allocation function 
            if (auto Call = dyn_cast<CallInst>(Inst)) {
              return; 
            }
          }
        }
        
        // for pointer aliasing
        std::string lval = dataflow::variable(receivingValue); 
        std::string rval = dataflow::variable(valueToStore);  

        if (lval[0] == rval[0] && lval[0] == '%') {
          this->aliasedVars[lval] = rval; 
        }
    

    
    }
    else if (auto Call = dyn_cast<CallInst>(I)) {
        for (unsigned i = 0; i < Call->getNumArgOperands(); ++i) {
            Value *argument = Call->getArgOperand(i);
            std::string argName = dataflow::variable(argument);

            argName = this->aliasedVars[argName];
            while (argName.size() > 1 && dataflow::isNumber(argName.substr(1))) {
                argName = this->aliasedVars[argName]; 
            }

            

            // * i THINK this is what distinguishes "real" variables defined in the program from constants and other irrelevant llvm stuff 
            if (argName[0] != '%') return; 

            std::list<std::string> allAliases;
            allAliases.push_back(argName); 
            while (argName != "") {
                argName = this->aliasedVars[argName]; 
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

                errs() << "CME-WARNING: unknown (unforseen occurence) & unsafe func on " << location << ". must call set for '" << argName << "' set to empty.\n";

                for (std::string aliasArg : allAliases) {
                    inputMethodsSet[aliasArg].methodsSet = {}; 
                }
                
                return; 
            }

            std::string fnName = Call->getCalledFunction()->getName().str();


            if (this->unsafeFunctions[fnName]) {
              logout("unsafe func, cm set emptied")
              for (std::string aliasArg : allAliases) {
                  inputMethodsSet[aliasArg].methodsSet = {}; 
              }

              return; 
            }

            if (this->reallocFunctions[fnName]) {
                logout("realloc func, cm set emptied")
                for (std::string aliasArg : allAliases) {
                    inputMethodsSet[aliasArg].methodsSet = {}; 
                }

                return; 
            }

            if (this->safeFunctions[fnName]) {
                logout("safe func")
                return; 
            }


            for (auto Pair : this->memoryFunctions) {
                if (fnName == Pair.first) { // allocation function

                }
                else if (fnName == Pair.second) { // deallocation function 
                    for (std::string aliasArg : allAliases) {
                      inputMethodsSet[aliasArg].methodsSet.insert(fnName); 
                      inputMethodsSet[aliasArg].setInitialized = true; 
                    }
                    
                    break; 

                }
            }
        }
    }
}



void CalledMethods::analyzeCFG(CFG* cfg, MappedMethods& PreMappedMethods, MappedMethods& PostMappedMethods, std::string priorBranch) {
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
    std::map<std::string, MethodsSetHolder> PriorPreCM = std::map<std::string, MethodsSetHolder>(PreMappedMethods[currentBranch]); 
    std::map<std::string, MethodsSetHolder> PriorPostCM = std::map<std::string, MethodsSetHolder>(PostMappedMethods[currentBranch]); 


    logout("-----\nlogging priorprecm prior = " << priorBranch << " current = " << currentBranch )
    for (auto p : PriorPreCM) {
      std::string m; 
      for (auto k : p.second.methodsSet) {
        m += k + ", ";
      }
      logout(p.first << " " << m)
    }


    if (PriorPreCM.size() > 0) {
      logout("need to lub for " << currentBranch << " " << priorBranch)

      std::map<std::string, MethodsSetHolder> CurrentPreCM = std::map<std::string, MethodsSetHolder>(PostMappedMethods[priorBranch]);
      

      // check if inputs (pre) differ
      bool currentAndPriorPreCMEqual = true; 
      for (auto Pair : CurrentPreCM) {
        std::string varName = Pair.first; 
        MethodsSetHolder currentPreCM = Pair.second; 

        if (
          (currentPreCM.methodsSet != PriorPreCM[varName].methodsSet) || 
          (currentPreCM.setInitialized != PriorPreCM[varName].setInitialized)
        ) {
          currentAndPriorPreCMEqual = false; 
          break; 
        }
      }

      for (auto Pair : PriorPreCM) {
        std::string varName = Pair.first; 
        MethodsSetHolder priorPreCM = Pair.second; 

        if (
          (priorPreCM.methodsSet != CurrentPreCM[varName].methodsSet) || 
          (priorPreCM.setInitialized != CurrentPreCM[varName].setInitialized)
        ) {
          currentAndPriorPreCMEqual = false; 
          break; 
        }

      }

      if (currentAndPriorPreCMEqual) return; 

      llvm::SetVector<Instruction*> instructions = cfg->getInstructions(); 
    
      // lub PriorPreCM and CurrentPreCM
      std::map<std::string, MethodsSetHolder> lub; 

      for (auto Pair1 : PriorPreCM) {
        std::string varName = Pair1.first; 
        MethodsSetHolder priorPreCM = Pair1.second; 
        
          
        // intersection 
        std::set<std::string> intersection;

        std::set_intersection(priorPreCM.methodsSet.begin(), priorPreCM.methodsSet.end(), 
                      CurrentPreCM[varName].methodsSet.begin(), CurrentPreCM[varName].methodsSet.end(), 
                      std::inserter(intersection, intersection.begin()));

        logout("for var name " << varName)

        std::string m,k; 
        for (auto i : priorPreCM.methodsSet) {
          m += i + ", ";
        }
        for (auto i : CurrentPreCM[varName].methodsSet) {
          k += i + ", ";
        }
        logout("prior pre cm = " << m)
        logout("current pre cm = " << k)


        lub[varName] = {
          intersection, 
          true
        }; 
        
      
      }

    
      // fill in the rest 
      for (auto Pair1 : PriorPostCM) {
        std::string varName = Pair1.first; 
        MethodsSetHolder priorPostCM = Pair1.second; 

        if (!lub[varName].setInitialized) {
          
          lub[varName] = {
            std::set<std::string>(priorPostCM.methodsSet), 
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
        logout("analyzing succesor " << succ->getBranchName())
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