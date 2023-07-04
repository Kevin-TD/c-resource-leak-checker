# run with `python3 ../run_all.py`  (must be in build dir)

import os
import sys

# checks if cwd is build
if os.getcwd().split("/")[2] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")

print("Making analysis")
os.system("make")


testFiles = os.listdir("../test")
testsPassed = True
for testFile in testFiles:
    splitFile = testFile.split(".")

    if (len(splitFile) != 2):
        continue

    fileName = splitFile[0]
    fileExtension = splitFile[1]

    if (fileExtension == "c"):
        if os.path.isfile(f"../test/{fileName}.txt"):
            command = f"clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c ../test/{fileName}.c -o ../test/{fileName}.ll ; opt -load CalledMethodsPass.so -CalledMethodsPass ../test/{fileName}.ll"
            print(f"running command {command}")
            exit_status = os.system(f"{command}")
            
            if exit_status != 0:
                testsPassed = False
        else:
            print(f"Test .txt file for {fileName} DOES NOT exist; no commands ran")

if testsPassed:
    sys.exit(0)
else:
    sys.exit(1)