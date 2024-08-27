cmake_minimum_required(VERSION 3.10)

find_package(LLVM 8 REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
include(AddLLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS} include)
link_directories(${LLVM_LIBRARY_DIRS} ${CMAKE_CURRENT_BINARY_DIR})


add_llvm_library(CodeAnalyzer MODULE
  src/Annotations/AnnotationHandler.cpp
  src/Annotations/Annotation.cpp
  src/Annotations/FunctionAnnotation.cpp
  src/Annotations/ParameterAnnotation.cpp
  src/Annotations/StructAnnotation.cpp
  src/Annotations/ReturnAnnotation.cpp 
  src/Annotations/ErrorAnnotation.cpp
  src/UtilFunctionTesters/RLCDataflow/VariableTester.cpp
  src/UtilFunctionTesters/RLCDataflow/UnwrapValuePointerToStructTester.cpp
  src/UtilFunctionTesters/RLCDataflow/IRstructNameEqualsCstructNameTester.cpp
  src/UtilFunctionTesters/RLCDataflow/GetPredecessorsTester.cpp
  src/UtilFunctionTesters/RLCDataflow/GetSuccessorsTester.cpp
  src/UtilFunctionTesters/RLCUtil/IsNumberTester.cpp
  src/UtilFunctionTesters/RLCUtil/SplitStringTester.cpp
  src/UtilFunctionTesters/RLCUtil/RemoveWhitespaceTester.cpp
  src/UtilFunctionTesters/RLCUtil/SliceStringTester.cpp
  src/UtilFunctionTesters/RLCUtil/IsValidCVariableNameTester.cpp
  src/UtilFunctionTesters/RLCUtil/HasOnlyOneBalancedParenthesesTester.cpp
  src/UtilFunctionTesters/RLCUtil/StartsWithTester.cpp
  src/UtilFunctionTesters/RLCUtil/SetToStringTester.cpp
  src/ProgramRepresentation/MethodsSet.cpp
  src/ProgramRepresentation/ProgramVariable.cpp
  src/ProgramRepresentation/PVAliasSet.cpp
  src/ProgramRepresentation/DisjointPVAliasSets.cpp
  src/ProgramRepresentation/ProgramPoint.cpp
  src/ProgramRepresentation/ProgramFunction.cpp
  src/ProgramRepresentation/FullFile.cpp
  src/Debug/BranchLister/LineBranchInfo.cpp
  src/Debug/BranchLister/ProgramLinesBranchInfo.cpp
  src/CFG.cpp
  src/CalledMethods.cpp
  src/MustCall.cpp
  src/DataflowPass.cpp
  src/TestRunner.cpp
  src/BranchListerTester.cpp
  src/StructFieldToIndexTester.cpp
  src/FunctionInfoTester.cpp
  src/StructFieldToIndexMap.cpp
  src/TempFileManager.cpp
  src/FunctionInfo.cpp
  src/FunctionInfosManager.cpp
  src/Analysis.cpp
  src/RunAnalysis.cpp
  src/Utils.cpp
)

target_compile_options(CodeAnalyzer PRIVATE -frtti)