#include "RunAnalysis.h"
#include "ProgramRepresentation/ProgramFunction.h"
#include "ProgramRepresentation/ProgramPoint.h"
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

void ResourceLeakScopeChecker::handleBranch(BasicBlock *B, ProgramBlock *blockMC, ProgramBlock *blockCM, AnnotationHandler *annos) {
    DisjointPVAliasSets dpvaMC, dpvaCM;

    ProgramPoint *pointMC, *pointCM;

    PVAliasSet *pvas, aliasMC, aliasCM;

    Value *retVal;

    // Iterate through call instructions and make sure the ownership makes sense

    int instNum = 1;
    for(Instruction &I : *B) {
        if (CallInst *call = dyn_cast<CallInst>(&I)) {
            pointMC = blockMC->getPoint(instNum);
            pointCM = blockCM->getPoint(instNum);

            std::string fnName = call->getCalledFunction()->getName().str();

            // For every parameter, check its annotations
            int argnum = -1;
            for(Value *arg : call->args()) {
                argnum += 1;
                Annotation *p = annos->getParameterAnnotation(fnName, argnum);
                if(ErrorAnnotation *e = dynamic_cast<ErrorAnnotation *>(p))
                    continue;
                switch(p->getAnnotationType()) {
                    // TODO: handle Annotations here, currently only one annotation is returned, I need to change
                    // it to be a list first
                }
            }
        }
        instNum += 1;
    }

    // Check if at every point, every alias either is satisfied or exists in the next point

    for(ProgramPoint *pointMC : blockMC->getPoints()) {
        pointCM = blockCM->getPoint(pointMC->getPointLine());
        if( pointCM == pointMC)
            llvm::errs() << "AAAAAAAAAAAAAAAAAAAAAAA"	<< "\n";
        //llvm::errs() << "\n\n\n MC IS \n";
        //ProgramPoint::logoutProgramPoint(pointMC, true);

        llvm::errs() << "\n\n\n CM IS \n";
        ProgramPoint::logoutProgramPoint(pointCM, true);
        for(PVAliasSet aliasMC : pointMC->getProgramVariableAliasSets().getSets()) {
            aliasCM = *pointCM->getPVASRef(aliasMC.getProgramVariables().front(), false);
            if(aliasMC.getMethodsSet().getMethods().size() == 0)
                continue;
            std::set<std::string> msMC = aliasMC.getMethodsSet().getMethods();
            std::set<std::string> msCM = aliasCM.getMethodsSet().getMethods();
            std::list<std::string> msetMC(msMC.begin(), msMC.end());
            std::list<std::string> msetCM(msCM.begin(), msCM.end());
            msetMC.sort();
            msetCM.sort();
            if(std::includes(msetCM.begin(), msetCM.end(), msetMC.begin(), msetMC.end())) {
                llvm::errs() << "FINISHED\n";
                // The obligation is satisfied, no further checks are needed
                continue;
            } else {
                // The obligation isn't satisfied, check if it exists in the next program point
                ProgramPoint *next = blockMC->getPoint(pointMC->getPointLine());
                if(next == pointMC || !next->getPVASRef(aliasMC.getProgramVariables().front(), false)) {
                    llvm::errs() << "End of life, failure for variable " << aliasMC.getProgramVariables().front().getCleanedName() << "\n";
                }
            }
        }
    }
    // TODO: Handle Return instructions, and handle blocks passing to other blocks
}

void ResourceLeakScopeChecker::doAnalysis(Function &F, ProgramFunction *pfMustCall, ProgramFunction *pfCalledMethods) {
    std::cout << "\n\n\n===\n\n\nCalled On \n\n\n===\n\n\n" << F.getName().str() << std::endl;

    std::list<ProgramBlock> programBlocksMC = pfMustCall->getProgramBlocks();
    std::list<ProgramBlock> programBlocksCM = pfCalledMethods->getProgramBlocks();
    BasicBlock *b;
    for(auto elementMC = programBlocksMC.begin(), elementCM = programBlocksCM.begin(); elementMC != programBlocksMC.end(); ++elementMC, ++elementCM) {

        // Find the corresponding block
        for(Function::iterator I = F.begin(), end = F.end(); end != I; ++I) {
            if(I->getName() == elementMC->getBlockName()) {
                b = &(*I);
                break;
            }
        }
        llvm::errs() << "\n\nBranch " << elementMC->getBlockName() << "\n";
        this->handleBranch(b, &*elementMC, &*elementCM, pfMustCall->getAnnotationHandler());
    }

}
}
