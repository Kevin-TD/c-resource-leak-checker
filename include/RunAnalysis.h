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
	class MustCall;
	class CalledMethods;

namespace rlc_dataflow {

// LLVM analysis results are in the form of structs, this struct will be defined
// as a result in the CodeAnalyzer struct and will be the return value of the doAnalysis function
struct CodeAnalyzerResult {
	CalledMethods *cm;
	MustCall *mc;
};

// This represents the analysis that constructs the must call and may called passes
// we will retrieve it's computation in Scope Analyzer to reason if/when to check obligations
struct CodeAnalyzer : public AnalysisInfoMixin<CodeAnalyzer> {
	public:
		static AnalysisKey Key;
		// Standard for llvm analysis, 
		using Result = CodeAnalyzerResult;

		CodeAnalyzerResult run(Function &F, FunctionAnalysisManager &FAM) {
    			doAnalysis(F, F.getParent()->getSourceFileName());
    			return {cm, mc};
		};

 	protected:
    		CalledMethods *cm;
    		MustCall *mc;
    		void doAnalysis(Function &F, std::string optLoadFileName);
    		void onEnd();
};

// This will be the actual pass ran, it will query the results of the CalledMethods and MustCall passes
// If we find that CalledMethods and MustCall must run over the entirety of the CFG before our scope analysis
// we can construct a pass before this one that is required by this one and which will get the results of CodeAnalyzer
// so that they will be cached for later use
struct ScopeAnalyzer : public PassInfoMixin<ScopeAnalyzer> {
	public:
		//TODO: This is only left here for developmental purposes and should be removed before release
                static bool isRequired() { return true; }
                PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
			auto c = FAM.getResult<CodeAnalyzer>(F);
                        doAnalysis(F, c.cm, c.mc);
                        return PreservedAnalyses::all();
                };
        protected:
                void doAnalysis(Function &F, CalledMethods *cm, MustCall *mc);
        };

} // namespace rlc_dataflow

#endif // RUN_ANALYSIS_H
