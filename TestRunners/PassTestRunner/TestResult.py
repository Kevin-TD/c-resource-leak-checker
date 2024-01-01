class TestResult:
    """Used to generate a string representing a test result, detailing 
    the test name, if it passed, and any additional notes. 
    test_has_passed, test_has_failed, or test_is_ignored must be 
    called at least once for a string to generate without error. 

    """
    __BOLD_ON = "\033[1m"
    __BOLD_OFF = "\033[22m"
    __RED_COLOR = "\033[31m"
    __GREEN_COLOR = "\033[32m"
    __RESET_COLOR = "\033[0m"


    __TEST_STATUS = {
        "UNSET": None,
        "IGNORED": "IGNORED",
        "PASSED": f"{__GREEN_COLOR}PASSED{__RESET_COLOR}", 
        "FAILED": f"{__RED_COLOR}FAILED{__RESET_COLOR}", 
    }

    def __init__(self, file_name: str):
        self.__file_name = file_name
        self.__test_notes: list[str] = []
        self.__test_status = self.__TEST_STATUS["UNSET"]
    
    def test_has_passed(self): 
        self.__test_status = self.__TEST_STATUS["PASSED"]
    
    def test_has_failed(self):
        self.__test_status = self.__TEST_STATUS["FAILED"]
    
    def test_is_ignored(self):
        self.__test_status = self.__TEST_STATUS["IGNORED"]
    
    def add_note(self, note: str):
        self.__test_notes.append(note)

    def to_string(self) -> str:
        """Returns test result as a string

        Raises:
            RuntimeError: error raised of test status never updated via 
            calling test_has_passed, test_has_failed, or test_is_ignored

        Returns:
            str
        """
        if self.__test_status == self.__TEST_STATUS["UNSET"]:
            raise RuntimeError("test status never updated to passed, failed, or ignored")
        
        notes_str = ("| " if len(self.__test_notes) else "")  + (" | ".join(self.__test_notes))

        return f"{self.__BOLD_ON}File{self.__BOLD_OFF} {self.__file_name} {self.__test_status} {notes_str}"