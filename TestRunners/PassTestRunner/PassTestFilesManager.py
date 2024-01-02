import sys

sys.path.insert(0, '..')
from TestRunners.TestFilesManager import TestFilesManager


class PassTestFilesManager(TestFilesManager):
    """ holds test files and provides helper functions to filter search 
    certain files or toggle file properties

    """

    def __init__(self):
        super().__init__()
        self.__make_will_be_called = True

    def make_will_be_called(self) -> bool:
        return self.__make_will_be_called

    def toggle_make_call(self):
        self.__make_will_be_called = not self.__make_will_be_called

    def toggle_ir_for(self, file_name: str):
        """Toggles IR generation for a file with name file_name

        Args:
            file_name (str): equal to file_path but with ../test/[test name]/ removed 

        Raises:
            FileNotFoundError: error raised if there is not a file with name file_name 
        """
        for test_file in self.__test_files:
            if test_file.get_file_name() == file_name:
                test_file.toggle_ir_generation()
                return

        raise FileNotFoundError(f"File '{file_name}' not found")

    def toggle_run_for(self, file_name: str):
        """Toggles test running for a file with name file_name

        Args:
            file_name (str): equal to file_path but with ../test/[test name]/ removed 

        Raises:
            FileNotFoundError: error raised if there is not a file with name file_name 
        """

        for test_file in self.__test_files:
            if test_file.get_file_name() == file_name:
                test_file.toggle_test_running()
                return

        raise FileNotFoundError(f"File '{file_name}' not found")