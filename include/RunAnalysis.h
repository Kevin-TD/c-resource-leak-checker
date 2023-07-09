#ifndef RUN_ANALYSIS_H
#define RUN_ANALYSIS_H

#include "llvm/ADT/SetVector.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include <algorithm>
#include <iterator>
#include <map>
#include <string>

using namespace llvm; 

namespace dataflow {

struct CalledMethodsAnalysis : public FunctionPass {
  static char ID;
  CalledMethodsAnalysis() : FunctionPass(ID) {}
  
  /**
   * This function is called for each function F in the input C program
   * that the compiler encounters during a pass.
   * You do not need to modify this function.
   */
  bool runOnFunction(Function &F) override;

protected:


  /**
   * @brief This function implements the chaotic iteration algorithm using
   * flowIn(), transfer(), and flowOut().
   *
   * @param F The function to be analyzed.
   */
  void doAnalysis(Function &F, std::string optLoadFileName);


  std::string getAnalysisName() { return "CalledMethodsPass"; }
};
} // namespace dataflow

#endif // RUN_ANALYSIS_H
