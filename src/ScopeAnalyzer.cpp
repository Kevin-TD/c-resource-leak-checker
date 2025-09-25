#include "RunAnalysis.h"
#include "MustCall.h"
#include "CalledMethods.h"
namespace rlc_dataflow {
	void ScopeAnalyzer::doAnalysis(Function &F, CalledMethods *cm, MustCall *mc) {
		std::cout << "\n\n\n===\n\n\nCalled On \n\n\n===\n\n\n" << F.getName().str() << std::endl;
	}
}
