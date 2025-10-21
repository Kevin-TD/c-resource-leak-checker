#include "RunAnalysis.h"
#include "MustCall.h"
#include "CalledMethods.h"

AnalysisKey rlc_dataflow::ResourceLeakFunctionAnalyzer::Key;

llvm::PassPluginLibraryInfo getCodeAnalyzerPluginInfo() {
    return { LLVM_PLUGIN_API_VERSION, "C Resource Leak Checker", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
        // Here we register our struct that determines which functions must have been called on resources and
        // which functions may need to be called to close the resource. This is an LLVM analysis which will only run
        // when requested for output, as oppposed to a pass, which will only run if we specifically call for it
        PB.registerAnalysisRegistrationCallback(
        [](FunctionAnalysisManager &FAM) {
            FAM.registerPass([&] {
                return rlc_dataflow::ResourceLeakFunctionAnalyzer();
            });
        });
        // Here we register the actual pass, the Name argument in the anonymous function is the name
        // the user provides as an argument to -passes= in clang
        PB.registerPipelineParsingCallback(
            [](StringRef Name, FunctionPassManager &FPM,
        ArrayRef<PassBuilder::PipelineElement>) {
            // TODO: This or statement keeps the CI and python testers working, it should be removed before release
            if(Name == "ResourceLeak" || "ScopeAnalyzer") {
                FPM.addPass(rlc_dataflow::ResourceLeakOutOfScopeDetector());
                return true;
            }
            return false;
        });
    }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getCodeAnalyzerPluginInfo();
}
