#!/bin/bash

# (in build dir)
# sh ../run_all.sh


echo "Making"
eval "make" 


for i in $(seq 1 15)
do
    command="clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test$i.c -o ../test/test$i.ll ; opt -load CalledMethodsPass.so -CalledMethodsPass ../test/test$i.ll"
   echo "Running command $command"
   eval $command 
done
