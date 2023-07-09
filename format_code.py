#! python3 ../run_all.py
# must be in /c-resource-leak-checker dir 

import os
import sys

os.system("find . -name '*.cpp' -o -name '*.h' | xargs clang-format -style=LLVM -i")