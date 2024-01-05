import sys

sys.path.insert(0, '..')
from TestRunners.Utils import *
from TestRunners.TestFile import TestFile


class AnnotationTestFile(TestFile):
    """ represents an c file from the /test directory
    """

    def __init__(self, file_path: str):
        super().__init__(file_path)
