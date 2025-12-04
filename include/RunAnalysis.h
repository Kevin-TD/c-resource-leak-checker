// c stdlib directives are for .cpp files

#ifndef RUN_ANALYSIS_H
#define RUN_ANALYSIS_H

#include "llvm/ADT/SetVector.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Transforms/Utils/Mem2Reg.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <regex>

using namespace llvm;
class ProgramFunction;
class ProgramPoint;
namespace rlc_dataflow {

// LLVM analysis results are in the form of structs, this struct will be defined
// as a result in the CodeAnalyzer struct and will be the return value of the doAnalysis function
struct ResourceLeakFunctionCallAnalyzerResult {
    ProgramFunction *programFunctionMustCall;
    ProgramFunction *programFunctionCalledMethods;
};

// This represents the analysis that constructs the must call and called method passes
// we will retrieve its computation in Scope Analyzer to reason if/when to check obligations
struct ResourceLeakFunctionCallAnalyzer : public AnalysisInfoMixin<ResourceLeakFunctionCallAnalyzer> {
  public:
    static AnalysisKey Key;
    // Standard for llvm analysis,
    using Result = ResourceLeakFunctionCallAnalyzerResult;

    ResourceLeakFunctionCallAnalyzerResult run(Function &F, FunctionAnalysisManager &FAM);
  protected:
    ResourceLeakFunctionCallAnalyzerResult doAnalysis(Function &F, std::string optLoadFileName);
    void onEnd();
};

// This will be the actual pass ran, it will query the results of the CalledMethods and MustCall passes
// If we find that CalledMethods and MustCall must run over the entirety of the CFG before our scope analysis
// we can construct a pass before this one that is required by this one and which will get the results of CodeAnalyzer
// so that they will be cached for later use
struct ResourceLeakScopeChecker : public PassInfoMixin<ResourceLeakScopeChecker> {
  public:
    //TODO: This is only left here for developmental purposes and should be removed before release
    static bool isRequired() {
        return true;
    }
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
        //PromotePass p;
        //p.run(F, FAM);
        auto c = FAM.getResult<ResourceLeakFunctionCallAnalyzer>(F);
        doAnalysis(F, c.programFunctionMustCall, c.programFunctionCalledMethods);
        return PreservedAnalyses::all();
    };
  protected:
    void doAnalysis(Function &F, ProgramFunction *pf, ProgramFunction *pf2);
    void handleBranch(Function &F, ProgramPoint *pmc, ProgramPoint *pcm);
    void onOutOfScope();
};

} // namespace rlc_dataflow

#endif // RUN_ANALYSIS_H
