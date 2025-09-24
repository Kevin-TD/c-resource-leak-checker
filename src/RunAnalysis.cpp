#include "RunAnalysis.h"
#include "MustCall.h"
#include "CalledMethods.h"

AnalysisKey rlc_dataflow::CodeAnalyzer::Key;

llvm::PassPluginLibraryInfo getCodeAnalyzerPluginInfo() {
	return { LLVM_PLUGIN_API_VERSION, "CodeAnalyzer", LLVM_VERSION_STRING, 
		[](PassBuilder &PB) {
			PB.registerAnalysisRegistrationCallback(
	 				[](FunctionAnalysisManager &FAM) {
						FAM.registerPass([&] {
								return rlc_dataflow::CodeAnalyzer();
							});
					});
			PB.registerPipelineParsingCallback(
					[](StringRef Name, FunctionPassManager &FPM,
						ArrayRef<PassBuilder::PipelineElement>) {
							if(Name == "ScopeAnalyzer") {
								FPM.addPass(rlc_dataflow::ScopeAnalyzer());
								return true;
							}
							return false;
					});
		}};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
	std::cout << "ENTERED HERE HELLO\n";
	return getCodeAnalyzerPluginInfo();
}
