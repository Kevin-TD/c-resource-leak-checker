import sys

sys.path.insert(0, '..')
from TestRunners.Utils import *
from TestRunners.TestFile import TestFile


class PassTestFile(TestFile):
    """ represents a C file from the test directory. holds information on whether its IR 
    will be generated or if that file will be ran. 
    """

    __INIT_PASS_TEST_CONDITIONS = {
        "IR_WILL_GENERATE": True
    }
    def __init__(self, file_path: str):
        super().__init__(file_path)
        self.__ir_will_generate: bool = self.__INIT_PASS_TEST_CONDITIONS["IR_WILL_GENERATE"]

    def toggle_ir_generation(self):
        self.__ir_will_generate = not self.__ir_will_generate

    def ir_will_generate(self) -> bool:
        return self.__ir_will_generate
