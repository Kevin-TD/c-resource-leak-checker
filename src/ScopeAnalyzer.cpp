#include "RunAnalysis.h"
#include "ProgramRepresentation/ProgramFunction.h"
#include "Utils.h"
namespace rlc_dataflow {


void ResourceLeakScopeChecker::onOutOfScope() {
    /*	if(!setMC->contains(retVal) && !includes(msetCM.begin(), msetCM.end(), msetMC.begin(), msetMC.end())) {
                //TODO: flesh out this error
                llvm::errs() << "ERROR!, Must Call not subset of Called Methods at " << F.getName().str() << "\n";
                llvm::errs() << "HERE IS Must Call " << setMC->getMethodsString() << "\n";
                llvm::errs() << "Here is CalledMethods " << asCM->getMethodsString() << "\n";
                llvm::errs() << "HERE ARE THE VARS: ";
                for(auto v : varsMC) {
                    llvm::errs() << v.getCleanedName() << ", ";
                }
                llvm::errs() << "\n";
            }*/
}

void ResourceLeakScopeChecker::handleBranch(Function &F, ProgramPoint *elementMC, ProgramPoint *elementCM) {
    DisjointPVAliasSets dpvaMC, dpvaCM;

    std::list<PVAliasSet> setsMC, setsCM;

    std::list<ProgramVariable> varsMC, varsCM;

    PVAliasSet *asCM;
    std::set<std::string> msetMC, msetCM;

    Value *retVal;
    llvm::errs() << "point " << elementMC->getPointName();
    // Go through instruction by instruction and create obligations (represented by PVAS)
    /*
    /*
    for(BasicBlock::iterator I = point->begin(), end = point->end(); end != I; ++I) {
    	if (StoreInst *store = dyn_cast<StoreInst>(I)) {
    	// Ignore this for now
    	// if the variable has not yet been used, then there is no need to check the consistency, it is not currently filled with anything to overwrite
            if (a != usedVariables.end()) {

    	}
    	else {
    		usedVariables.push_back(store->getOperand(1)->getName.str());
    	}
    }
    }

        // We want to check if the resource runs "out of scope" in the next function for every set of aliases
        for(auto setMC = setsMC.begin(); setMC != setsMC.end(); ++setMC) {
            varsMC = setMC->getProgramVariables();
            if(varsMC.size() == 0)
                continue;
            asCM = dpvaCM.findMatchingSet(varsMC);
            msetCM = asCM->getMethodsSet().getMethods();
            msetMC = setMC->getMethodsSet().getMethods();
            bool continues_in_successor = 0;
            bool continues_in_all =  elementMC->getSuccessors().size() > 0;
            for(auto succ : elementMC->getSuccessors()) {
                continues_in_successor = 0;
                for(auto var : varsMC) {
                    if(succ->getPVASRef(var, false)) {
                        continues_in_successor = 1;
                        break;
                    }
                }
                continues_in_all = continues_in_all && continues_in_successor;
                if(!continues_in_all)
                    break;
            }
            if(continues_in_all) {
                continue;
            }

            if(!setMC->contains(retVal) && !includes(msetCM.begin(), msetCM.end(), msetMC.begin(), msetMC.end())) {
                //TODO: flesh out this error
                llvm::errs() << "ERROR!, Must Call not subset of Called Methods at " << F.getName().str() << "\n";
                llvm::errs() << "HERE IS Must Call " << setMC->getMethodsString() << "\n";
                llvm::errs() << "Here is CalledMethods " << asCM->getMethodsString() << "\n";
                llvm::errs() << "HERE ARE THE VARS: ";
                for(auto v : varsMC) {
                    llvm::errs() << v.getCleanedName() << ", ";
                }
                llvm::errs() << "\n";
            }
        }
    Needs to be rewritten with new ProgramBlocks*/
}

void ResourceLeakScopeChecker::doAnalysis(Function &F, ProgramFunction *pfMustCall, ProgramFunction *pfCalledMethods) {
    std::cout << "\n\n\n===\n\n\nCalled On \n\n\n===\n\n\n" << F.getName().str() << std::endl;

    //std::list<ProgramPoint> programPointsMC = pfMustCall->getProgramPoints();
    //std::list<ProgramPoint> programPointsCM = pfCalledMethods->getProgramPoints();
    /*

    for(auto elementMC = programPointsMC.begin(), elementCM = programPointsCM.begin(); elementMC != programPointsMC.end(); ++elementMC, ++elementCM) {
       this->handleBranch(F, &*elementMC, &*elementCM);
    }*/
}

}
