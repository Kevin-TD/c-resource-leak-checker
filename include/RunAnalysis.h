// c stdlib directives are for .cpp files

#ifndef RUN_ANALYSIS_H
#define RUN_ANALYSIS_H

#include "llvm/ADT/SetVector.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Pass.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
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

using namespace llvm;

namespace dataflow {

struct CodeAnalyzer : public FunctionPass {
  static char ID;
  CodeAnalyzer() : FunctionPass(ID) {}

  /**
   * This function is called for each function F in the input C program
   * that the compiler encounters during a pass.
   */
  bool runOnFunction(Function &F) override;

  // calls when all functions have been passed
  bool doFinalization(Module &M) override;

protected:
  void doAnalysis(Function &F, std::string optLoadFileName);
  void onEnd();

  std::string getAnalysisName() { return "CalledMethodsPass"; }
};
} // namespace dataflow

#endif // RUN_ANALYSIS_H
