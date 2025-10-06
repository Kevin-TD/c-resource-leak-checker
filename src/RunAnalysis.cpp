#include "RunAnalysis.h"
#include "MustCall.h"
#include "ProgramRepresentation/ProgramFunction.h"
#include "ProgramRepresentation/ProgramPoint.h"
#include "CalledMethods.h"

AnalysisKey rlc_dataflow::ResourceLeakFunctionCallAnalyzer::Key;

rlc_dataflow::ResourceLeakFunctionCallAnalyzerResult rlc_dataflow::ResourceLeakFunctionCallAnalyzer::run(Function &F, FunctionAnalysisManager &FAM) {
    auto ret = doAnalysis(F, F.getParent()->getSourceFileName());
    ret.programFunctionMustCall->getProgramPoints();
    return ret;
};


llvm::PassPluginLibraryInfo getResourceLeakFunctionCallAnalyzerPluginInfo() {
    return { LLVM_PLUGIN_API_VERSION, "ResourceLeakFunctionCallAnalyzer", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
        // Here we register our analysis as an analysis, which will always be registered, as oppposed
        // to a pass, which will only sometimes be registered if we specifically call for it
        PB.registerAnalysisRegistrationCallback(
        [](FunctionAnalysisManager &FAM) {
            FAM.registerPass([&] {
                return rlc_dataflow::ResourceLeakFunctionCallAnalyzer();
            });
        });
        // Here we register the actual pass, the Name argument in the anonymous function is the name
        // the user provides as an argument to -passes= in clang
        PB.registerPipelineParsingCallback(
            [](StringRef Name, FunctionPassManager &FPM,
        ArrayRef<PassBuilder::PipelineElement>) {
            if(Name == "ScopeAnalyzer") {
                FPM.addPass(rlc_dataflow::ResourceLeakScopeChecker());
                return true;
            }
            return false;
        });
    }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    std::cout << "ENTERED HERE HELLO\n";
    return getResourceLeakFunctionCallAnalyzerPluginInfo();
}
