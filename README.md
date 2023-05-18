# C-resource-leak-checker

How to run the analysis 
1. set dir to build 
2. run any of following in terminal (first for test1, second for test2. identical commands, just different file names)

cmake .. ; make ; clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test1.c -o ../test/test1.ll ; opt -load MustCallPass.so -MustCallEstimates ../test/test1.ll 

make .. ; make ; clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test2.c -o ../test/test2.ll ; opt -load MustCallPass.so -MustCallEstimates ../test/test2.ll 

To re-building everything, just add a "make clean" to the list of commands 

make clean; cmake .. ; make ; clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test1.c -o ../test/test1.ll ; opt -load MustCallPass.so -MustCallEstimates ../test/test1.ll 
