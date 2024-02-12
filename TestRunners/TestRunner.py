from abc import ABC, abstractmethod
from .FlagsManager import FlagsManager
from .TestFilesManager import TestFilesManager
from .Utils import *
from .TestResult import TestResult
import sys


class TestRunner(ABC):
    """Abstract class that manages user input and executes tests. 
    The call to manage_input_and_run_tests runs the test based on sys args
    and returns with an exit status, 0 indicating success and 1 indicating failure. 
    Results are printed at the end of its execution.

    """

    def __init__(self, flags_manager: FlagsManager, test_files_manager: TestFilesManager):
        """
        Args:
            flags_manager (FlagsManager)
            test_files_manager (TestFilesManager): Must be a sub class of TestFilesManager

        Raises:
            ValueError: errors if test_files_manager is not a sub class of TestFilesManager
        """
        self._flags_manager: FlagsManager = flags_manager
        self._commands_did_not_fail: bool = True
        self._results: "list[str]" = []

        if not issubclass(type(test_files_manager), TestFilesManager):
            raise ValueError(
                f"Type of test_files_manager ({type(test_files_manager)}) is not sub class of TestFilesManager")

        self._test_files_manager = test_files_manager
        self._exit_status = None

    @abstractmethod
    def _add_c_file(self, c_file: str):
        """Gives the path of a C file in order to be inserted to self._test_files_manager using 
        the correct constructor. E.g., pass test needs to add it via PassTestFile(c_file), and 
        annotation test needs to add it via AnnotationTestFile(c_file)

        Args:
            c_file (str): path to a C file
        """
        pass

    @abstractmethod
    def _before_tests_run(self):
        """Code ran right before the iteration of C files to test  
        """
        pass

    @abstractmethod
    def _on_tests_run(self, test_file, test_result: TestResult, test_folder_name: str):
        """Code ran right before there is a check to see if a test will run (i.e., 
        calling test_file.test_will_run())

        Args:
            test_file: file type will be based on type of test_files_manager. E.g., if the type of test_files_manager
            is PassTestFilesManager, then test_file will be PassTestFile
            test_result (TestResult)
            test_folder_name (str)
        """
        pass

    @abstractmethod
    def _on_test_will_run(self, test_file, test_result: TestResult, test_folder_name: str):
        """Code that is ran if test will run 

        Args:
            test_file: file type will be based on type of test_files_manager. E.g., if the type of test_files_manager
            is PassTestFilesManager, then test_file will be PassTestFile
            test_result (TestResult)
            test_folder_name (str)
        """
        pass

    def manage_input_and_run_tests(self, system_args: "list[str]"):
        """ Handles user input of test folder name and flags as described by 
        system_args and runs tests accordingly. The method does the following:
        1) Checks if test folder input refers to an existing directory and returns the flag manager's to_string if it does not
        2) Checks if input is -h or --help
        3) Applies actions to test files as described by flags 
        4) Calls before_tests_run
        5) Starts iterating over c files in test directory
        6) Calls on_tests_run 
        7) Checks if tests has failed and skips to next test if it has
        8) Calls on_test_will_run if test will run

        The exit status is returned, with 0 indicating success and 1 indicating failure

        Args:
            system_args (list[str])

        Returns:
            int: exit staus (0 = success; 1 = failure)
        """
        if os.path.split(os.getcwd())[1] != "build":
            print(f"ERROR: not in build dir; cwd is {os.getcwd()}")
            sys.exit(1)

        if len(system_args) == 1:
            print(self._flags_manager.to_string())
            return 1

        # for sys.argv, 0th = ../run_test.py, 1st = folder name, 2 and beyond = user args
        test_folder_name = system_args[1]

        if (test_folder_name == "-h" or test_folder_name == "--help"):
            print(self._flags_manager.to_string())
            return 0

        try:
            c_files = get_all_c_files(f"../test/{test_folder_name}")
        except Exception as e:
            print(e)
            print(self._flags_manager.to_string())
            return 1

        for c_file in c_files:
            self._add_c_file(c_file)

        sys_arg_iterator = 2

        self._flags_manager.check_for_error_pair(sys.argv[2:])

        while sys_arg_iterator < len(sys.argv):
            arg = sys.argv[sys_arg_iterator]

            if (self._flags_manager.flag_exists(arg)):
                flag = self._flags_manager.get_flag(arg)

                action_arg_count = flag.get_action().__code__.co_argcount

                if (action_arg_count == 1):
                    flag.get_action()(self._test_files_manager)
                elif (action_arg_count == 2):
                    try:
                        next_arg = sys.argv[sys_arg_iterator + 1]
                    except IndexError:
                        raise FileNotFoundError(
                            f"c file not specified for flag {arg}")

                    flag.get_action()(self._test_files_manager, next_arg)
                    sys_arg_iterator += 2
                    continue

            else:
                raise ValueError(
                    f"Flag {arg} not found. {self._flags_manager.to_string()}")

            sys_arg_iterator += 1

        self._before_tests_run()

        for c_file in c_files:
            # n = 3 for find_nth since c_file is in the format
            # ../test/{test_name}/{.c file}
            start_index_of_c_file_name = find_nth(c_file, "/", 3) + 1

            f = self._test_files_manager.get_file(
                c_file[start_index_of_c_file_name:])

            test_result = TestResult(c_file)

            self._on_tests_run(f, test_result, test_folder_name)

            if test_result.has_failed():
                continue

            print(f"Running Test: {f.test_will_run()}")

            if f.test_will_run():
                self._on_test_will_run(f, test_result, test_folder_name)
            else:
                test_result.ignore_test()

            self._results.append(test_result.to_string())

        print("---------RUN RESULTS---------")
        for result in self._results:
            print(result)

        if self._commands_did_not_fail:
            return 0
        else:
            return 1
