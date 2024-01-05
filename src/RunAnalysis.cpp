#include "RunAnalysis.h"

namespace rlc_dataflow {

bool CodeAnalyzer::runOnFunction(Function &F) {
    doAnalysis(F, F.getParent()->getSourceFileName());

    return false;
}

bool CodeAnalyzer::doFinalization(Module &M) {
    onEnd();
    return false;
}

char CodeAnalyzer::ID = 1;
static RegisterPass<CodeAnalyzer> X("CodeAnalyzer", "Code analyzer", false,
                                    false);
} // namespace rlc_dataflow
