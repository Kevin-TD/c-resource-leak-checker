#! python3 check_formatting.py
# must be in ./c-resource-leak-checker

import os
import sys

# checks if cwd is /c-resource-leak-checker
if os.path.split(os.getcwd())[1] != "c-resource-leak-checker":
    print(
        f"WARNING: not in dir /c-resource-leak-checker; cwd is {os.getcwd()}")
    sys.exit(1)

if os.system("! astyle --dry-run --style=google --exclude=build --ignore-exclude-errors --recursive *.cpp,*.h,*.c -n | grep Formatted") != 0:
    print(".cpp, .h, and .c files unformatted - checked with astyle")
    sys.exit(1)

if os.system("! autopep8 -d -r --ignore=E402 . | grep fixed") != 0:
    print(".py files unformatted - checked with autopep8")
    sys.exit(1)
else:
    sys.exit(0)
