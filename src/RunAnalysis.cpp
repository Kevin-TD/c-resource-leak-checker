#include "RunAnalysis.h"
#include "MustCall.h"
#include "CalledMethods.h"


llvm::PassPluginLibraryInfo getCodeAnalyzerPluginInfo() {
    return { LLVM_PLUGIN_API_VERSION, "C Resource Leak Checker", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
        PB.registerPipelineParsingCallback(
            [](StringRef Name, ModulePassManager &MPM,
        ArrayRef<PassBuilder::PipelineElement>) {
            MPM.addPass(createModuleToFunctionPassAdaptor(rlc_dataflow::CodeAnalyzer()));
            return true;
        });
    }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getCodeAnalyzerPluginInfo();
}

namespace rlc_dataflow {

} // namespace rlc_dataflow
