cmake_minimum_required(VERSION 3.10)

find_package(LLVM 8 REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions)
include(AddLLVM)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS} include)
link_directories(${LLVM_LIBRARY_DIRS} ${CMAKE_CURRENT_BINARY_DIR})


add_llvm_library(CalledMethodsPass MODULE
  src/ProgramVariable.cpp
  src/Annotation.cpp
  src/FunctionAnnotation.cpp
  src/ParameterAnnotation.cpp
  src/ReturnAnnotation.cpp 
  src/StructAnnotation.cpp
  src/ErrorAnnotation.cpp
  src/ProgramVariablesHandler.cpp
  src/CFG.cpp
  src/CalledMethods.cpp
  src/MustCall.cpp
  src/DataflowPass.cpp
  src/TestRunner.cpp
  src/Analysis.cpp
  src/RunAnalysis.cpp
  src/Utils.cpp
)

target_compile_options(CalledMethodsPass PRIVATE -frtti)