#! python3 format_code.py
# must be in ./c-resource-leak-checker

import os
import sys

# checks if cwd is /c-resource-leak-checker
if os.path.split(os.getcwd())[1] != "c-resource-leak-checker":
    print(f"WARNING: not in dir /c-resource-leak-checker; cwd is {os.getcwd()}")
    sys.exit(1)

os.system("find . -path ./build -prune -o \( -name '*.cpp' -o -name '*.h' \) -print | xargs clang-format-11 -style=LLVM -i")