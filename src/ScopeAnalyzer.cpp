#include "RunAnalysis.h"
#include "MustCall.h"
#include "CalledMethods.h"
namespace rlc_dataflow {
	void ScopeAnalyzer::doAnalysis(Function &F, CalledMethods *cm, MustCall *mc) {
		std::cout << "Called On " << F.getName().str() << std::endl;
	}
}
