#include "RunAnalysis.h"

// LLVM IR typically names the first branch of a block "entry"
const std::string FIRST_BRANCH_NAME = "entry";

const std::string CALLED_METHODS_PASS_NAME = "CalledMethods";
const std::string MUST_CALL_PASS_NAME = "MustCall";
const std::string GENERATED_DIR_NAME = "CodeAnalyzerFiles";

const std::string LLVM_PTR_ANNOTATION = "llvm.ptr.annotation";
const std::string LLVM_VAR_ANNOTATION = "llvm.var.annotation";
const std::string LLVM_DBG_DECLARE = "llvm.dbg.declare";

// relative to build dir
const std::string PASS_TEST_DIRNAME = "../Testers/Passes";
const std::string BRANCH_LISTER_TEST_DIRNAME = "../Testers/BranchLister";

const std::string AST_ANNO_PASS_LOCATION = "../ASTAnalyses/ASTPasses/AnnotationPass/anno_pass.py";
const std::string AST_INFO_GENERATOR_LOCATION = "../ASTAnalyses/ASTInfo/generator.py";
const std::string AST_FIELD_TO_INDEX_MAP_GENERATOR_LOCATION = "../ASTAnalyses/ASTPasses/FieldToIndexPass/field_to_index_pass.py";
const std::string AST_FUNCTION_INFO_PASS_LOCATION = "../ASTAnalyses/ASTPasses/FunctionInfoPass/get_function_info.py";
const std::string AST_L_VALUE_PASS_LOCATION = "../ASTAnalyses/ASTPasses/LValuePass/l_value_pass.py";