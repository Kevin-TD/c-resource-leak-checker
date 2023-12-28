#! python3 ../run_test.py <folder name in /test> 
# must be in build dir
# e.g., python3 ../run_test.py nesting_test 

# takes a test folder in the /test directory and runs every .c 
# file in it, including those in subdirectories 

import os 
import sys 
from multiprocessing import Process

# checks if cwd is build
if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)

# TODO: add command line arg support to specify if we dont want to 
# call make or  re-make IR. also modify if we want to exclude certain files. 
# TODO: there should be final display of all the test results at the end
# TODO: update annotation tester to be as robust as run_test 
    

test_folder_name = sys.argv[1]

'''
(plans for command line tool impl)
(should also say for results at the end if we did not build ir or explicitly excluded it or did not make)

syntax: 
python3 ../run_test.py [file name] [flags]
python3 ../run_test.py or python3 ../run_test.py --help or python3 ../run_test.py -h     gives you the help 


typical usage
python3 ../run_test.py simple_ptr_test

extra usage 
python3 ../run_test.py simple_ptr_test --no-make --no-build-ir
python3 ../run_test.py simple_layer_test --only-run-for test1.c
python3 ../run_test.py simple_layer_test --only-run-for layer/test1_again.c

commands
--no-make: does not run make 

--no-build-ir: does not build ir for any files. assumes it exists already and will error if they do not 

--no-build-ir-for [c file name] does not build ir for specified file 
    for like simple_layer_test, if you wanted to ignore test1.c, just enter test1.c. if nested,
    type layer/test1_again.c
--only-build-ir-for [c file name]    can be ran for multiple files. ignores all except the ones specified by --only-build-ir-for

--no-run-for [c file name]
--only-run-for [c file name]

should error for the following combinations:  
• --no-build-ir and --only-build-ir-for [file]
• --no-build-ir-for [A] and --only-build-ir-for [B] where A = B 
• --no-run-for [A] and --only-run-for [B] where A = B


class TestFile:
    string file_name
    bool ir_generation
    bool test_ran 



'''

def get_all_c_files(folder_path: str, collected_files = []):
    with os.scandir(folder_path) as entries:
        for entry in entries:
            if entry.is_file() and entry.name.endswith(".c"):
                collected_files.append(f"{folder_path}/{entry.name}")

            elif entry.is_dir():
                get_all_c_files(f"{folder_path}/{entry.name}", collected_files)
    
    return collected_files


c_files = get_all_c_files(f"../test/{test_folder_name}")

print("Making analysis")
make_status = os.system("make")

if make_status != 0:
    sys.exit(make_status)

commands_did_not_fail = True

commands = []

for c_file in c_files:
    c_file_as_ll = c_file.replace(".c", ".ll")
    command = f"clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c {c_file} -o {c_file_as_ll} ; opt -load CodeAnalyzer.so -CodeAnalyzer {c_file_as_ll}"
    print(f"Running command: {command}")
    exit_status = os.system(command)
        
    if exit_status != 0:
        commands_did_not_fail = False
        commands.append(f"test {c_file} failed - exit status {exit_status}")
    else:
        commands.append(f"test {c_file} did not fail")

print("---------RUN RESULTS---------")
for result in commands:
    print(result)

if commands_did_not_fail:
    sys.exit(0)
else:
    sys.exit(1)