import sys
import os
import tempfile

sys.path.insert(0, '..')
from TestRunners.TestRunner import TestRunner
from TestRunners.TestResult import TestResult
from TestRunners.FlagsManager import FlagsManager
from TestRunners.TestFilesManager import TestFilesManager
from AnnotationTestFile import AnnotationTestFile


class AnnotationTestRunner(TestRunner):
    def __init__(self, flags_manager: FlagsManager, test_files_manager: TestFilesManager):
        super().__init__(flags_manager, test_files_manager)

    def _add_c_file(self, c_file):
        self._test_files_manager.add_file(AnnotationTestFile(c_file))

    def _before_tests_run(self):
        pass

    def _on_tests_run(self, test_file: AnnotationTestFile, test_result: TestResult, test_folder_name: str):
        pass

    def _on_test_will_run(self, test_file: AnnotationTestFile, test_result: TestResult, test_folder_name: str):
        c_file = test_file.get_file_path()
        test_text_file = "../Testers/Annotations/" + test_folder_name + \
            "/" + test_file.get_file_name().replace(".c", ".txt")

        print(c_file, test_text_file)
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
                self._commands_did_not_fail = False
                test_result.test_has_failed(
                    f"annotation build error - exit status {build_status}")
            elif diff_status != 0:
                self._commands_did_not_fail = False
                test_result.test_has_failed(
                    f"diff annotation - output:\n{out}")
            else:
                test_result.test_has_passed()
