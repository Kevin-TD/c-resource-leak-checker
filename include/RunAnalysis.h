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

struct CodeAnalyzerResult {
	CalledMethods *cm;
	MustCall *mc;
};
struct CodeAnalyzer : public AnalysisInfoMixin<CodeAnalyzer> {
	public:
		static AnalysisKey Key;
		// Standard for llvm analysis
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

struct ScopeAnalyzer : public PassInfoMixin<ScopeAnalyzer> {
	public:
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
