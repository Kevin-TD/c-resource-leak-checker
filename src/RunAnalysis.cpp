#include "RunAnalysis.h"

namespace dataflow {

bool CodeAnalyzer::runOnFunction(Function &F) {
  buildAST(F.getParent()->getSourceFileName());
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
} // namespace dataflow
