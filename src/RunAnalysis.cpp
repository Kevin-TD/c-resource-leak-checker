#include "RunAnalysis.h"
#include "MustCall.h"
#include "CalledMethods.h"

AnalysisKey rlc_dataflow::CodeAnalyzer::Key;

llvm::PassPluginLibraryInfo getCodeAnalyzerPluginInfo() {
	return { LLVM_PLUGIN_API_VERSION, "CodeAnalyzer", LLVM_VERSION_STRING, 
		[](PassBuilder &PB) {
			// Here we register our analysis as an analysis, which will always be registered, as oppposed
			// to a pass, which will only sometimes be registered if we specifically call for it
			PB.registerAnalysisRegistrationCallback(
	 				[](FunctionAnalysisManager &FAM) {
						FAM.registerPass([&] {
								return rlc_dataflow::CodeAnalyzer();
							});
					});
			// Here we register the actual pass, the Name argument in the anonymous function is the name
			// the user provides as an argument to -passes= in clang
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
