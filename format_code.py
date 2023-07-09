#! python3 format_code.py
# must be in ./c-resource-leak-checker

import os

# checks if cwd is build
if os.path.split(os.getcwd())[1] != "c-resource-leak-checker":
    print(f"WARNING: not in dir /c-resource-leak-checker; cwd is {os.getcwd()}")

os.system("find . -path ./build -prune -o \( -name '*.cpp' -o -name '*.h' \) -print | xargs clang-format -style=LLVM -i ")