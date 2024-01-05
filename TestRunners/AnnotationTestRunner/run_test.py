# annotation_tester.py runs every test. this runs for single file
#  clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/double_mustcall/index.c > AST.txt ; python3 ../Annotations/annotation_generator.py AST.txt ANNO.txt

import os
import sys

sys.path.insert(0, '..')
from TestRunners.FlagsManager import FlagsManager
from AnnotationTestFileManager import AnnotationTestFileManager
from TestRunners.Utils import *
from TestRunners.TestResult import TestResult
from AnnotationTestFile import AnnotationTestFile
import tempfile

# TODO: run_test here and run_test in PassTestRunner share code. generalize it
# TODO: write syntax comment (reword to 'usage examples'?)

if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)

if __name__ == "__main__":
    '''
    syntax: 
    '''

    flag_managers = FlagsManager(
        "python3 ../run_at.py [required test folder name] [optional flags]")
    flag_managers.add_flag(
        "h", "help",
        "Show help"
    )

    flag_managers.add_flag(
        "r", "no-run-for",
        "Does not check annotations for specified c file. Can be called multiple times. c file must be specified in the succeeding argument. Combined with --only-run-for may result in undesirable output.",
        lambda annotation_test_files_manager, file_name: annotation_test_files_manager.get_file(
            file_name).toggle_test_running()
    )

    flag_managers.add_flag(
        "n", "only-run-for",
        "Only runs specified c file. c file must be specified in the succeeding argument. Combined with --no-run-for may result in undesireable output.",
        lambda annotation_test_files_manager, file_name: [file.toggle_test_running(
        ) for file in annotation_test_files_manager.get_all_files_excluding(file_name)]
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

    annotation_test_files_manager = AnnotationTestFileManager()
    for c_file in c_files:
        annotation_test_files_manager.add_file(AnnotationTestFile(c_file))

    sys_arg_iterator = 2
    while sys_arg_iterator < len(sys.argv):
        arg = sys.argv[sys_arg_iterator]

        if (flag_managers.flag_exists(arg)):
            flag = flag_managers.get_flag(arg)

            action_arg_count = flag.get_action().__code__.co_argcount

            if (action_arg_count == 1):
                flag.get_action()(annotation_test_files_manager)
            elif (action_arg_count == 2):
                try:
                    next_arg = sys.argv[sys_arg_iterator + 1]
                except IndexError:
                    raise FileNotFoundError(
                        f"c file not specified for flag {arg}")

                flag.get_action()(annotation_test_files_manager, next_arg)
                sys_arg_iterator += 2
                continue

        else:
            raise ValueError(
                f"Flag {arg} not found. {flag_managers.to_string()}")

        sys_arg_iterator += 1

    results: "list[str]" = []

    commands_did_not_fail = True

    for c_file in c_files:

        f = annotation_test_files_manager.get_file(
            c_file[find_nth(c_file, "/", 3) + 1:])

        test_result = TestResult(c_file)

        print(f"Running Test: {f.test_will_run()}")

        if f.test_will_run():
            test_text_file = "../Testers/Annotations/" + test_folder_name + \
                "/" + f.get_file_name().replace(".c", ".txt")

            if not os.path.isfile(test_text_file):
                test_result.test_is_ignored()
                test_result.add_note("no test written")
            else:
                with tempfile.NamedTemporaryFile(prefix="temp_ast_input", suffix=".txt", delete=False) as ast_input:
                    ast_input_name = ast_input.name
                with tempfile.NamedTemporaryFile(prefix="temp_anno_output", suffix=".txt", delete=False) as annotations_output:
                    annotations_output_name = annotations_output.name

                command = f"clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics {c_file} > {ast_input_name} ; python3 ../Annotations/annotation_generator.py {ast_input_name} {annotations_output_name}"
                print(f"Running build command")
                build_status = os.system(command)

                diff_command = f"diff {test_text_file} {annotations_output_name}"

                print(f"Running diff command")
                diff_status = os.system(diff_command)

                with open(annotations_output_name, "r") as f:
                    out = f.read()

                os.remove(ast_input_name)
                os.remove(annotations_output_name)

                if build_status != 0:
                    commands_did_not_fail = False
                    test_result.test_has_failed(
                        f"annotation build error - exit status {build_status}")
                elif diff_status != 0:
                    commands_did_not_fail = False
                    test_result.test_has_failed(
                        f"diff annotation - output:\n{out}")
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
