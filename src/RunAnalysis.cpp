#include "RunAnalysis.h"

namespace dataflow {

bool CalledMethodsAnalysis::runOnFunction(Function &F) {
  auto PA = new PointerAnalysis(F);
  doAnalysis(F, PA, F.getParent()->getSourceFileName());

  return false;
}

char CalledMethodsAnalysis::ID = 1;
static RegisterPass<CalledMethodsAnalysis> X("CalledMethodsPass", "Called methods analysis",
                                       false, false);
} // namespace dataflow
