# C-resource-leak-checker

This analysis runs with docker; here are the steps to run the analysis: 

1. Clone the repo and set directory to the repo's 
2. Pull the docker image: ```docker pull cis547/cis547-base:latest```
3. Run docker in interactive mode: ```docker run -it -v $(pwd):/$(basename $(pwd)) -w /$(basename $(pwd))  cis547/cis547-base:latest```
4. Run the following to get opt working: ```export LD_LIBRARY_PATH=/opt/mirtk/lib:${LD_LIBRARY_PATH}```
- To be honest, I am not sure how this makes opt works but it just does. 
5. Make (```mkdir build```) and set directory to build (```cd build```)
6. Build Clang, LLVM, other important stuff with ```cmake ..```
7. Build analysis with ```make```
8. Generate LLVM IR with ```clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test2.c -o ../test/test2.ll```  (change test files ```./test/test2.c``` and ```./test/test2.ll``` accordingly)
9. Load the results with ```opt -load CalledMethodsPass.so -CalledMethodsPass ../test/test2.ll ```

Make further changes to repo in the include and src directories, and for future changes, just run ```make ; clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/test2.c -o ../test/test2.ll ; opt -load CalledMethodsPass.so -CalledMethodsPass ../test/test2.ll ``` (with test files changes accordingly).
Alternatively, just run the run_test.sh file (```sh ../run_test.sh <test number>```); e.g, ```sh ../run_test.sh 1``` will run the analysis for test/test1.c

- Note: if any files in your cwd have spaces in them, then step 3 will likely fail as the ```$(basename $(pwd))``` command will not work as intended. This is an issue to fix later, but for now, just make sure you don't have any spaces in your folders