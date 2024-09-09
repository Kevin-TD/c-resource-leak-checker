class LValue:
    """Represents an l-value reference in the C code for LHS assignments or return statements
    """

    def __init__(self, lhs_name: str, line_number: int):
        self.__lhs_name = lhs_name
        self.__line_number = line_number

    def get_lhs_name(self):
        return self.__lhs_name

    def set_lhs_name(self, lhs_name):
        self.__lhs_name = lhs_name

    def get_line_number(self):
        return self.__line_number
