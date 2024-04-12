#include "RunAnalysis.h"

// LLVM IR typically names the first branch of a block "entry"
const std::string FIRST_BRANCH_NAME = "entry";

const std::string CALLED_METHODS_PASS_NAME = "CalledMethods";
const std::string MUST_CALL_PASS_NAME = "MustCall";
const std::string GENERATED_DIR_NAME = "CodeAnalyzerFiles";

const std::string LLVM_PTR_ANNOTATION = "llvm.ptr.annotation";
const std::string LLVM_VAR_ANNOTATION = "llvm.var.annotation";

// relative to build dir
const std::string AST_ANNO_PASS_FILE_LOCATION = "python3 ../ASTAnalyses/ASTPasses/AnnotationPass/anno_pass.py";
const std::string AST_INFO_GENERATOR_LOCATION = "python3 ../ASTAnalyses/ASTInfo/generator.py";
const std::string AST_FIELD_TO_INDEX_MAP_GENERATOR_LOCATION = "python3 ../TestHelpers/struct_field_to_index_map_generator.py";