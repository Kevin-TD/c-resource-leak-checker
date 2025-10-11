#include "RunAnalysis.h"
#include "ProgramRepresentation/ProgramFunction.h"
namespace rlc_dataflow {


void ResourceLeakScopeChecker::doAnalysis(Function &F, ProgramFunction *pfMustCall, ProgramFunction *pfCalledMethods) {
    std::cout << "\n\n\n===\n\n\nCalled On \n\n\n===\n\n\n" << F.getName().str() << std::endl;

    std::list<ProgramPoint> programPointsMC = pfMustCall->getProgramPoints();
    std::list<ProgramPoint> programPointsCM = pfCalledMethods->getProgramPoints();

    DisjointPVAliasSets dpvaMC, nextDPVAMC;
    DisjointPVAliasSets dpvaCM;

    std::list<PVAliasSet> setsMC, setsCM;

    ProgramPoint currentMC, nextMC;
    ProgramPoint currentCM;

    std::list<ProgramVariable> varsMC, varsCM;

    nextMC = programPointsMC.front();
    for(auto elementMC = ++programPointsMC.begin(), elementCM = programPointsCM.begin(); elementMC != programPointsMC.end(); ++elementMC, ++elementCM) {
        currentMC = nextMC;
        nextMC = *elementMC;

        currentCM = *elementCM;

        dpvaMC = currentMC.getProgramVariableAliasSets();
        nextDPVAMC = nextMC.getProgramVariableAliasSets();
        setsMC = dpvaMC.getSets();
        // We want to check if the resource runs "out of scope" in the next function for every set of aliases
        for(auto setMC = setsMC.begin(); setMC != setsMC.end(); ++setMC) {
            //TODO: Finish checking if the next branch has elements in scope
            std::cout << "COUNT\n";
        }
    }

    // At the end of the function, the only elements in scope "after" will be that which is returned
    // TODO: Ask about global scope (assume unhandled?)
    currentMC = programPointsMC.back();
    currentCM = programPointsCM.back();

    dpvaMC = currentMC.getProgramVariableAliasSets();
    dpvaCM = currentCM.getProgramVariableAliasSets();

    setsMC = dpvaMC.getSets();
    setsCM = dpvaCM.getSets();

    PVAliasSet *asCM;

    std::set<std::string> msetMC, msetCM;

    for(auto setMC = setsMC.begin(); setMC != setsMC.end(); ++setMC) {
        varsMC = setMC->getProgramVariables();
        asCM = dpvaCM.findMatchingSet(varsMC);
        msetCM = asCM->getMethodsSet().getMethods();
        msetMC = (*setMC).getMethodsSet().getMethods();
        if(!includes(msetCM.begin(), msetCM.end(), msetMC.begin(), msetMC.end())) {
            //TODO: flesh out this error
            llvm::errs() << "ERROR!, Must Call not subset of Called Methods at " << F.getName().str() << "\n";
            llvm::errs() << "HERE IS Must Call " << setMC->getMethodsString() << "\n";
            llvm::errs() << "Here is CalledMethods " << asCM->getMethodsString() << "\n";
        }
    }

    std::cout << "DONE\n" << std::endl;
}

}
