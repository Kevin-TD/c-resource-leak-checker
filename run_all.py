#! python3 ../run_all.py
# must be in build dir

import os
import sys


results = []
tests_that_did_not_run = []

# checks if cwd is build
if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)


def get_all_c_files(folder_path: str, collected_files=[]):
    with os.scandir(folder_path) as entries:
        for entry in entries:
            if entry.is_file() and entry.name.endswith(".c"):
                collected_files.append(f"{folder_path}/{entry.name}")

            elif entry.is_dir():
                get_all_c_files(f"{folder_path}/{entry.name}", collected_files)

    return collected_files


print("---------BEGINNING CODE TEST---------")
print("Making analysis")
make_status = os.system("make")

if make_status != 0:
    sys.exit(make_status)

tests_passed = True

with os.scandir("../test") as entries:
    for entry in entries:

        if entry.is_dir():
            c_files = get_all_c_files(f"../test/{entry.name}", [])

            for c_file in c_files:
                c_file_no_extension = c_file[:-2]
                c_file_txt = c_file_no_extension + ".txt"
                c_file_ll = c_file_no_extension + ".ll"

                test_file = "../Testers/Passes/" + \
                    c_file_no_extension.replace("../test/", "") + ".txt"

                if os.path.isfile(test_file):
                    command = f"clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c {c_file} -o {c_file_ll} ; opt -load CodeAnalyzer.so -CodeAnalyzer {c_file_ll}"
                    print(f"Running command: {command}")
                    exit_status = os.system(command)

                    if exit_status != 0:
                        tests_passed = False
                        results.append(f"{test_file} failed")
                    else:
                        results.append(f"{test_file} passed")

                else:
                    tests_that_did_not_run.append(test_file)

print("---------TEST RESULTS---------")
for result in results:
    print(result)


if (len(tests_that_did_not_run) > 0):
    print("---------DID NOT RUN---------")
for test in tests_that_did_not_run:
    print(test)

print("---------BEGINNING ANNOTATION TEST---------")
print("Running command: python3 ../annotation_tester.py")
annotation_test = os.system("python3 ../annotation_tester.py")

if annotation_test != 0:
    tests_passed = False

if tests_passed:
    sys.exit(0)
else:
    sys.exit(1)
