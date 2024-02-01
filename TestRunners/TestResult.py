from .Utils import TERMINAL_TEXT_STYLES


class TestResult:
    """Used to generate a string representing a test result, detailing 
    the test name, if it passed, and any additional notes. 
    test_has_passed, test_has_failed, or test_is_ignored must be 
    called at least once for a string to generate without error. 

    """

    __TEST_STATUS = {
        "UNSET": None,
        "IGNORED": "IGNORED",
        "PASSED": f"{TERMINAL_TEXT_STYLES['GREEN_COLOR']}PASSED{TERMINAL_TEXT_STYLES['RESET_COLOR']}",
        "FAILED": f"{TERMINAL_TEXT_STYLES['RED_COLOR']}FAILED{TERMINAL_TEXT_STYLES['RESET_COLOR']}",
    }

    def __init__(self, file_name: str):
        self.__file_name = file_name
        self.__test_notes: list[str] = []
        self.__test_status = self.__TEST_STATUS["UNSET"]
        self.__object_type = "File"

    def set_object_type_to_folder(self):
        """When to_string is called, test will be referred to as a folder 
        """
        self.__object_type = "Folder"

    def set_object_type_to_file(self):
        """"When to_string is called, test will be referred to as a file. 
        This is the default behavior and does not need to be called if the test 
        is intended to represent a file and not a folder. 
        """
        self.__object_type = "File"

    def test_has_passed(self, note=""):
        if note != "":
            self.add_note(note)

        self.__test_status = self.__TEST_STATUS["PASSED"]

    def test_has_failed(self, note=""):
        if note != "":
            self.add_note(note)

        self.__test_status = self.__TEST_STATUS["FAILED"]

    def check_if_test_has_failed(self) -> bool:
        return self.__test_status == self.__TEST_STATUS["FAILED"]

    def test_is_ignored(self, note=""):
        if note != "":
            self.add_note(note)

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
            raise RuntimeError(
                "test status never updated to passed, failed, or ignored")

        notes_str = ("| " if len(self.__test_notes) else "") + \
            (" | ".join(self.__test_notes))

        object_type_str = TERMINAL_TEXT_STYLES['BOLD_ON'] + \
            self.__object_type + TERMINAL_TEXT_STYLES['BOLD_OFF']

        return f"{object_type_str} {self.__file_name} {self.__test_status} {notes_str}"
