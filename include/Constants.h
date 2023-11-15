#include "RunAnalysis.h"

// LLVM IR typically names the first branch of a block "entry"
const std::string FIRST_BRANCH_NAME = "entry";

const std::string CALLED_METHODS_PASS_NAME = "CalledMethods";
const std::string MUST_CALL_PASS_NAME = "MustCall";
const std::string GENERATED_DIR_NAME = "CodeAnalyzerFiles";

const std::string LLVM_PTR_ANNOTATION = "llvm.ptr.annotation";
const std::string LLVM_VAR_ANNOTATION = "llvm.var.annotation";