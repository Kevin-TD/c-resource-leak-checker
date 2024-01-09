#! python3 ../whole_test_run.py [test runner folder name] [expected result folder name] [optional --run-if-no-test] [additional flags for test runner]
# must be in build dir

# --run-if-no-test flag will make it so a test directory will be tested a
# anyways even if no tests are written for it. by default, if no test
# dir exists, the test is not ran.

# additional flags are found by calling -h on test runner's run_test.py file.
# the flags are applied to all calls to test folders

# Example Usage:
# python3 ../whole_test_run.py AnnotationTestRunner Annotations
# python3 ../whole_test_run.py PassTestRunner Passes
# python3 ../whole_test_run.py PassTestRunner Passes --run-if-no-test
# python3 ../whole_test_run.py PassTestRunner Passes --run-if-no-test --no-make

# test runner name is based on what is found in /TestRunners
# runs the run_test.py file in the test runner for each test in /test
# then prints the output

# TODO: in ci call make then call test with no make

import os
import sys

sys.path.insert(0, '..')
from TestRunners.Utils import *
from TestRunners.TestResult import TestResult

# checks if cwd is build
if os.path.split(os.getcwd())[1] != "build":
    print(f"WARNING: not in build dir; cwd is {os.getcwd()}")
    sys.exit(1)

test_runner_name = sys.argv[1]
expected_result_name = sys.argv[2]


extra_flags = ""
no_run_if_no_test = True

if len(sys.argv) > 3:
    if sys.argv[3] == "--run-if-no-test":
        no_run_if_no_test = False

        if len(sys.argv) > 4:
            extra_flags = " ".join(sys.argv[4:])
    else:
        extra_flags = " ".join(sys.argv[3:])

results: "list[str]" = []
tests_passed = True

with os.scandir("../test") as entries:
    for entry in entries:
        if entry.is_dir():
            test_folder_name = entry.name

            test_result = TestResult(test_folder_name)
            test_result.set_object_type_to_folder()

            expected_result_folder = f"../Testers/{expected_result_name}/{test_folder_name}"

            no_test_written = not os.path.isdir(expected_result_folder)

            if no_test_written:
                test_result.add_note("no test written")

            if not no_run_if_no_test or not no_test_written:
                exit_status = os.system(
                    f"python3 ../TestRunners/{test_runner_name}/run_test.py {test_folder_name} {extra_flags}")

                if exit_status != 0:
                    tests_passed = False
                    test_result.test_has_failed()
                else:
                    test_result.test_has_passed()
            else:
                test_result.test_is_ignored()

            results.append(test_result.to_string())


print("---------WHOLE TEST RUN RESULTS---------")
for result in results:
    print(result)

if tests_passed:
    sys.exit(0)
else:
    sys.exit(1)
