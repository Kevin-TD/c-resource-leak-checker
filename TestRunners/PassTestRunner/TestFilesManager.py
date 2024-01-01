from TestFile import TestFile

class TestFilesManager:
    """ holds test files and provides helper functions to filter search 
    certain files or toggle file properties

    """
    def __init__(self):
        self.__test_files: list[TestFile.TestFile] = []
        self.__make_will_be_called = True
    
    def add_file(self, file_path: str):
        self.__test_files.append(TestFile(file_path))
    
    def get_file(self, file_name: str) -> TestFile:
        """Gets file reference based on file name

        Args:
            file_name (str): equal to file_path but with ../test/[test name]/ removed 

        Raises:
            FileNotFoundError: error raised if file is not found based on file_name

        Returns:
            TestFile
        """
        for test_file in self.__test_files:
            if test_file.get_file_name() == file_name:
                return test_file
        
        raise FileNotFoundError(f"File '{file_name}' not found")
    
    def get_all_files_excluding(self, file_name: str) -> "list[TestFile.TestFile]":
        """Returns a list of refernces to files which do not have name file_name

        Args:
            file_name (str): equal to file_path but with ../test/[test name]/ removed 

        Raises:
            FileNotFoundError: error raised there is not a file with name file_name

        Returns:
            list[TestFile.TestFile]
        """
        test_files = []
        file_found = False
        for test_file in self.__test_files:
            if test_file.get_file_name() != file_name:
                test_files.append(test_file)
            else: 
                file_found = True
        
        if not file_found:
            raise FileNotFoundError(f"File '{file_name}' not found")
        
        return test_files

    def get_all_files(self) -> "list[TestFile.TestFile]":
        """ returns a list of references to the files

        Returns:
            list[TestFile.TestFile]
        """
        test_files = []
        for test_file in self.__test_files:
            test_files.append(test_file)
        
        return test_files
    
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