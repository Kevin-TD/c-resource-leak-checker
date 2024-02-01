import sys
import os

sys.path.insert(0, '..')
from TestRunners.TestRunner import TestRunner
from TestRunners.TestResult import TestResult
from TestRunners.FlagsManager import FlagsManager
from TestRunners.TestFilesManager import TestFilesManager
from PassTestFile import PassTestFile


class PassTestRunner(TestRunner):
    def __init__(self, flags_manager: FlagsManager, test_files_manager: TestFilesManager):
        super().__init__(flags_manager, test_files_manager)

    def _add_c_file(self, c_file):
        self._test_files_manager.add_file(PassTestFile(c_file))

    def _before_tests_run(self):
        print(
            f"Calling Make: {self._test_files_manager.make_will_be_called()}")

        if (self._test_files_manager.make_will_be_called()):
            make_status = os.system("make")

            if make_status != 0:
                sys.exit(make_status)
        else:
            self._results.append("make not called")

    def _on_tests_run(self, test_file: PassTestFile, test_result: TestResult, test_folder_name: str):
        c_file = test_file.get_file_path()
        c_file_as_ll = c_file.replace(".c", ".ll")

        make_ir_command = f"clang -emit-llvm -g -S -fno-discard-value-names -Xclang -disable-O0-optnone -c {c_file} -o {c_file_as_ll}"

        if test_file.ir_will_generate():
            ir_generate_exit_status = os.system(make_ir_command)

            if ir_generate_exit_status != 0:
                self._commands_did_not_fail = False

                test_result.test_has_failed(
                    f"failed @ IR generation - exit status {ir_generate_exit_status}")

                self._results.append(test_result.to_string())

    def _on_test_will_run(self, test_file: PassTestFile, test_result: TestResult, test_folder_name: str):
        c_file = test_file.get_file_path()
        c_file_as_ll = c_file.replace(".c", ".ll")
        test_text_file = "../Testers/Passes/" + test_folder_name + \
            "/" + test_file.get_file_name().replace(".c", ".txt")

        if not test_file.ir_will_generate():
            test_result.add_note("ir was not generated")

        run_test_command = f"opt -load CodeAnalyzer.so -CodeAnalyzer {c_file_as_ll}"
        test_run_exit_status = os.system(run_test_command)

        if test_run_exit_status != 0:
            self._commands_did_not_fail = False

            test_result.test_has_failed(
                f"failed @ running - exit status {test_run_exit_status}")
        else:
            if not os.path.isfile(test_text_file):
                test_result.test_is_ignored("no test written")
            else:
                test_result.test_has_passed()
