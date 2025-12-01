# CMAKE based off of the llvm tutor repo found here:
# https://github.com/banach-space/llvm-tutor/blob/a62756e265d2d28930634a932122fc2a2ce193ef/CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(c-resource-checker)

set(LT_LLVM_INCLUDE_DIR "/usr/lib/llvm-13/include/llvm")
set(LT_LLVM_INSTALL_DIR "/usr/lib/llvm-13/")

if(NOT EXISTS "${LT_LLVM_INCLUDE_DIR}")
message(FATAL_ERROR
  " LT_LLVM_INSTALL_DIR (${LT_LLVM_INCLUDE_DIR}) is invalid.")
endif()

set(LT_VALID_INSTALLATION FALSE)

# Ubuntu + Darwin
if(EXISTS "${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/LLVMConfig.cmake")
  set(LT_VALID_INSTALLATION TRUE)
endif()

# Fedora
if(EXISTS "${LT_LLVM_INSTALL_DIR}/lib64/cmake/llvm/LLVMConfig.cmake")
  set(LT_VALID_INSTALLATION TRUE)
endif()

if(NOT ${LT_VALID_INSTALLATION})
	message("${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/LLVMConfig.cmake")
  message(FATAL_ERROR
    "LLVM installation directory, (${LT_LLVM_INSTALL_DIR}), is invalid. Couldn't
    find LLVMConfig.cmake.")
endif()

list(APPEND CMAKE_PREFIX_PATH "${LT_LLVM_INSTALL_DIR}/lib/cmake/llvm/")

find_package(LLVM 13 CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LT_LLVM_INSTALL_DIR}")

message("LLVM STATUS:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}
  ")
 
include_directories(SYSTEM include ${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE
      STRING "Build type (default Debug):" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall\
  -fdiagnostics-color=always")

if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-fvisibility-inlines-hidden" SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG)
if (${SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG} EQUAL "1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden")
endif()

add_library(CodeAnalyzer SHARED
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
  src/UtilFunctionTesters/RLCUtil/StartsWithTester.cpp
  src/UtilFunctionTesters/RLCUtil/SetToStringTester.cpp
  src/ProgramRepresentation/MethodsSet.cpp
  src/ProgramRepresentation/ProgramVariable.cpp
  src/ProgramRepresentation/PVAliasSet.cpp
  src/ProgramRepresentation/DisjointPVAliasSets.cpp
  src/ProgramRepresentation/ProgramPoint.cpp
  src/ProgramRepresentation/ProgramBlock.cpp
  src/ProgramRepresentation/ProgramFunction.cpp
  src/ProgramRepresentation/FullFile.cpp
  src/Debug/BranchLister/LineBranchInfo.cpp
  src/Debug/BranchLister/ProgramLinesBranchInfo.cpp
  src/CFG.cpp
  src/CalledMethods.cpp
  src/MustCall.cpp
  src/DataflowPass.cpp
  src/ScopeAnalyzer.cpp
  src/TestRunner.cpp
  src/BranchListerTester.cpp
  src/StructFieldToIndexTester.cpp
  src/FunctionInfoTester.cpp
  src/StructFieldToIndexMap.cpp
  src/LineNumberToLValueMap.cpp
  src/LineNumberToLValueTester.cpp
  src/TempFileManager.cpp
  src/FunctionInfo.cpp
  src/FunctionInfosManager.cpp
  src/Analysis.cpp
  src/RunAnalysis.cpp
  src/Utils.cpp
)
