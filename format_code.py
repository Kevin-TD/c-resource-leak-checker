#! python3 format_code.py
# must be in ./c-resource-leak-checker

import os
import sys

# checks if cwd is /c-resource-leak-checker
if os.path.split(os.getcwd())[1] != "c-resource-leak-checker":
    print(
        f"WARNING: not in dir /c-resource-leak-checker; cwd is {os.getcwd()}")
    sys.exit(1)

os.system("astyle --style=google --exclude=build --ignore-exclude-errors --recursive *.cpp,*.h,*.c -n")
os.system("autopep8 --in-place --recursive --ignore=E402 .")
