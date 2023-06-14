#!/bin/bash

# (in build dir)
# sh ../run_test.sh <test number>
# sh ../run_test.sh 1

# command="make ; clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test$1.c -o ../test/test$1.ll ; opt -load CalledMethodsPass.so -CalledMethodsPass ../test/test$1.ll"

command="make ; clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test$1.c -o ../test/test$1.ll ; opt -load CalledMethodsPass.so -CalledMethodsPass ../test/test$1.ll"


echo "Running command: $command"

eval $command