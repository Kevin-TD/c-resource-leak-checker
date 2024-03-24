class StructVar:
    """Represents a variable declared in the C code that has a struct type
    """
    def __init__(self, var_name: str, type_name: str):
        self.__var_name = var_name
        self.__type_name = type_name

    def get_var_name(self):
        return self.__var_name

    def get_type_name(self):
        return self.__type_name