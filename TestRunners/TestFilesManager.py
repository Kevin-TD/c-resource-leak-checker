from .TestFile import TestFile


class TestFilesManager:
    """ generic class that provides structure for holding test files and 
    provides helper functions to filter search certain files or toggle file properties.
    must use a subclass of TestFile, which is declared by a call to add_file then 
    subsequently enforced. 

    """

    def __init__(self):
        self.__test_files: [TestFile] = []
        self.__declared_subclass = None

    def add_file(self, file_object: [TestFile]):
        """ Stores file_object internally. All insertions must be a subclass of TestFile. 
        The first insertion determines what subclass type is enforced for further 
        insertions. 

        Args:
            file_object (TestFile.TestFile]): must be a subclass of TestFile
        """
        if self.__declared_subclass is None:
            file_object_type = type(file_object)

            if issubclass(file_object_type, TestFile):
                self.__declared_subclass = file_object_type
            else:
                raise ValueError(
                    f"Type of file_object ({file_object_type}) is not sub class of TestFile")
        else:
            if not type(file_object) is self.__declared_subclass:
                raise ValueError(
                    f"Type of file_object ({type(file_object)}) is equal to {self.__declared_subclass}")

        self.__test_files.append(file_object)

    def get_file(self, file_name: str):
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
