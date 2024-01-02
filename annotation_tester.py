#! python3 ../annotation_tester.py
# must be in build dir

import os 
import sys 
import tempfile

results = []
tests_that_did_not_run = []

DEBUG = True

def logout(x: str):
    if (DEBUG):
        print(x)

# checks if cwd is build
if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)


def get_all_c_files(folder_path: str, collected_files = []):
    with os.scandir(folder_path) as entries:
        for entry in entries:
            if entry.is_file() and entry.name.endswith(".c"):
                collected_files.append(f"{folder_path}/{entry.name}")

            elif entry.is_dir():
                get_all_c_files(f"{folder_path}/{entry.name}", collected_files)
    
    return collected_files


tests_passed = True

with os.scandir("../test") as entries:
    for entry in entries:

        if entry.is_dir():
            c_files = get_all_c_files(f"../test/{entry.name}", [])

            for c_file in c_files:
                c_file_no_extension = c_file[:-2]
                c_file_txt = c_file_no_extension + ".txt"
                c_file_ll = c_file_no_extension + ".ll"

                test_file = "../Testers/Annotations/" + c_file_no_extension.replace("../test/", "") + ".txt"

                if os.path.isfile(test_file):
                    with tempfile.NamedTemporaryFile(prefix="temp_ast_input", suffix=".txt", delete=False) as ast_input:
                        ast_input_name = ast_input.name 
                    with tempfile.NamedTemporaryFile(prefix="temp_anno_output", suffix=".txt", delete=False) as annotations_output:
                        annotations_output_name = annotations_output.name 

                    command = f"clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics {c_file} > {ast_input_name} ; python3 ../Annotations/annotation_generator.py {ast_input_name} {annotations_output_name}"
                    print(f"Running build command: {command}")
                    build_status = os.system(command)

                    diff_command = f"diff {test_file} {annotations_output_name}"
                    print(f"Running diff command: {diff_command}")
                    diff_status = os.system(diff_command)

                    with open(annotations_output_name, "r") as f:
                        out = f.read()
                    
                    os.remove(ast_input_name)
                    os.remove(annotations_output_name)

                    if build_status != 0:
                        tests_passed = False
                        results.append(f"{test_file} failed - annotation build error")
                    elif diff_status != 0:
                        logout(f"DIFF ANNOTATION RECEIVED:\n{out}")
                        tests_passed = False 
                        results.append(f"{test_file} failed - diff annotation")
                    else:
                        results.append(f"{test_file} passed")

                    
                else:
                    tests_that_did_not_run.append(test_file)

print("---------ANNOTATION TEST RESULTS---------")
for result in results:
    print(result)


if (len(tests_that_did_not_run) > 0):
    print("---------DID NOT RUN---------")
    
for test in tests_that_did_not_run:
    print(test)

if tests_passed:
    sys.exit(0)
else:
    sys.exit(1)