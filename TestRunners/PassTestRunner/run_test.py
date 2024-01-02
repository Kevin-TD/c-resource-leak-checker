#! python3 ../TestRunners/PassTestRunner/run_test.py <folder name in /test> 
# must be in build dir
# e.g., python3 ../TestRunners/PassTestRunner/run_test.py nesting_test 

# takes a test folder in the /test directory and runs every .c 
# file in it, including those in subdirectories 

# TODO: make "run_all.py" run this script. also run_all.py needs to be updated
# make this change AFTER merging into main/more neat pr
# TODO: make scripts for AnnotationTestRunner
# TODO: use python script formatter & add ci test

import os 
import sys 

sys.path.insert(0, '..')
from TestRunners.FlagsManager import FlagsManager
from PassTestFilesManager import PassTestFilesManager
from TestRunners.Utils import *
from TestRunners.TestResult import TestResult
from PassTestFile import PassTestFile

if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)

if __name__ == "__main__":
    '''
    syntax: 
    python3 ../TestRunners/PassTestRunner/run_test.py [file name] [flags]
    python3 ../TestRunners/PassTestRunner/run_test.py -h to get help

    example usage
    python3 ../TestRunners/PassTestRunner/run_test.py simple_ptr_test
    python3 ../TestRunners/PassTestRunner/run_test.py simple_ptr_test --no-make --no-build-ir
    python3 ../TestRunners/PassTestRunner/run_test.py simple_layer_test --only-run-for test1.c
    python3 ../TestRunners/PassTestRunner/run_test.py simple_layer_test --only-run-for layer/test1_again.c
    '''

    flag_managers = FlagsManager("python3 ../TestRunners/PassTestRunner/run_test.py [required test folder name] [optional flags]")
    flag_managers.add_flag(
        "h", "help",
        "Show help"
    )

    flag_managers.add_flag(
        "m", "no-make",
        "Does not call 'make' command", 
        lambda pass_test_files_manager : pass_test_files_manager.toggle_make_call()
    )

    flag_managers.add_flag(
        "b", "no-build-ir", 
        "Does not build IR for any file. Combined with --only-build-ir-for may result in undesirable output as these commands are opposites. ", 
        lambda pass_test_files_manager : [file.toggle_ir_generation() for file in pass_test_files_manager.get_all_files()]
    )

    flag_managers.add_flag(
        "f", "no-build-ir-for", 
        "Does not build IR for the specified c file. Can be called multiple times. c file must be specified in the succeeding argument, or else there will be an error.", 
        lambda pass_test_files_manager, file_name: pass_test_files_manager.get_file(file_name).toggle_ir_generation()

    )
    flag_managers.add_flag(
        "u", "only-build-ir-for", 
        "Only builds ir for specified c file. c file must be specified in the succeeding argument, or else there will be an error. Note that c files will still be ran unless otherwise disallowed (use -n). Combined with --no-build-ir may result in undesirable output as these commands are opposites.",
        lambda pass_test_files_manager, file_name : [file.toggle_ir_generation() for file in pass_test_files_manager.get_all_files_excluding(file_name)]
    )

    flag_managers.add_flag(
        "r", "no-run-for", 
        "Does not specified c file. Can be called multiple times. c file must be specified in the succeeding argument, or else there will be an error.", 
        lambda pass_test_files_manager, file_name: pass_test_files_manager.get_file(file_name).toggle_test_running()   
    )

    flag_managers.add_flag(
        "n", "only-run-for", 
        "Only runs specified c file. c file must be specified in the succeeding argument, or else there will be an error. Note that IR will still generate unless otherwise disallowed (use -u).",
        lambda pass_test_files_manager, file_name : [file.toggle_test_running() for file in pass_test_files_manager.get_all_files_excluding(file_name)]
    )

    if len(sys.argv) == 1:
        print(flag_managers.to_string())
        sys.exit(1)

    # for sys.argv, 0th = ../run_test.py, 1st = folder name, 2 and beyond = user args

    test_folder_name = sys.argv[1]

    if (test_folder_name == "-h" or test_folder_name == "--help"):
        print(flag_managers.to_string())
        sys.exit(0)
    
    try:
        c_files = get_all_c_files(f"../test/{test_folder_name}")
    except Exception as e:
        print(e)
        print(flag_managers.to_string())
        sys.exit(1)

    pass_test_files_manager = PassTestFilesManager()
    for c_file in c_files:
        pass_test_files_manager.add_file(PassTestFile(c_file))

    sys_arg_iterator = 2
    while sys_arg_iterator < len(sys.argv):
        arg = sys.argv[sys_arg_iterator]

        if (flag_managers.flag_exists(arg)):
            flag = flag_managers.get_flag(arg)

            action_arg_count = flag.get_action().__code__.co_argcount

            if (action_arg_count == 1):
                flag.get_action()(pass_test_files_manager)
            elif (action_arg_count == 2):
                try: 
                    next_arg = sys.argv[sys_arg_iterator + 1]
                except IndexError:
                    raise FileNotFoundError(f"c file not specified for flag {arg}")

                flag.get_action()(pass_test_files_manager, next_arg)
                sys_arg_iterator += 2
                continue

        else:
            raise ValueError(f"Flag {arg} not found. {flag_managers.to_string()}")

        sys_arg_iterator += 1

    print(f"Calling Make: {pass_test_files_manager.make_will_be_called()}")

    results: "list[str]" = []

    if (pass_test_files_manager.make_will_be_called()):
        make_status = os.system("make")

        if make_status != 0:
            sys.exit(make_status)
    else:
        results.append("make not called")

    commands_did_not_fail = True


    for c_file in c_files:

        c_file_as_ll = c_file.replace(".c", ".ll")

        f = pass_test_files_manager.get_file(c_file[find_nth(c_file, "/", 3) + 1:])

        test_result = TestResult(c_file)

        make_ir_command = f"clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c {c_file} -o {c_file_as_ll}"
        run_test_command = f"opt -load CodeAnalyzer.so -CodeAnalyzer {c_file_as_ll}"

        print(f"Making IR: {f.ir_will_generate()}")

        if f.ir_will_generate():
            ir_generate_exit_status = os.system(make_ir_command)

            if ir_generate_exit_status != 0:
                commands_did_not_fail = False

                test_result.test_has_failed()
                test_result.add_note(f"failed @ IR generation - exit status {ir_generate_exit_status}")

                results.append(test_result.to_string())
                continue
        
        print(f"Running Test: {f.test_will_run()}")

        if f.test_will_run():
            if not f.ir_will_generate():
                test_result.add_note("ir was not generated")

            test_run_exit_status = os.system(run_test_command)

            if test_run_exit_status != 0:
                commands_did_not_fail = False

                test_result.test_has_failed()
                test_result.add_note(f"failed @ running - exit status {test_run_exit_status}")
                
            else:
                test_text_file = "../Testers/Passes/" + test_folder_name + "/" + f.get_file_name().replace(".c", ".txt")
                
                if not os.path.isfile(test_text_file):
                    test_result.test_is_ignored()
                    test_result.add_note("no test written")
                else:
                    test_result.test_has_passed()

        else:
            test_result.test_is_ignored()        
        
        results.append(test_result.to_string())


    print("---------RUN RESULTS---------")
    for result in results:
        print(result)

    if commands_did_not_fail:
        sys.exit(0)
    else:
        sys.exit(1)