from Specifiers.Specifier import Specifier
from Specifiers.FunctionStructure.Function import Function
from Specifiers.StructStructure.Struct import Struct
from ASTInfoGenerator.Debug import *


class SpecifierManager:
    """Manages a list of specifiers
    """

    def __init__(self):
        self.__specifiers: list[Specifier] = []

    # if function is not found, a new one is returned with name function_name
    def get_function(self, function_name: str) -> Function:
        for specifier in self.__specifiers:
            if type(specifier) is Function:
                if specifier.equals(function_name):
                    return specifier

        new_function = Function(function_name, None)
        self.__specifiers.append(new_function)
        return self.__specifiers[-1]

    # if struct is not found, a new one is returned with name struct_name if error_if_not_found is false
    def get_struct(self, struct_name: str, error_if_not_found: bool = False) -> Struct:
        for specifier in self.__specifiers:
            if type(specifier) is Struct:
                if specifier.equals(struct_name):
                    return specifier

        if error_if_not_found:
            raise ValueError(f"struct {struct_name} not found")

        new_struct = Struct(struct_name)
        self.__specifiers.append(new_struct)
        return self.__specifiers[-1]

    # if there already exists a function of name function_name, that function is returned.
    # otherwise, the function is added and returned.
    def add_function(self, function_name: str, return_type: str) -> Specifier:
        for specifier in self.__specifiers:
            if type(specifier) is Function:
                if specifier.equals(function_name):
                    return specifier

        new_specifier = Function(function_name, return_type)
        self.__specifiers.append(new_specifier)
        return self.__specifiers[-1]

    # if there already exists a struct of name struct_name, that struct is returned.
    # otherwise, the struct is added and returned.
    def add_struct(self, struct_name: str) -> bool:
        for specifier in self.__specifiers:
            if type(specifier) is Struct:
                if specifier.equals(struct_name):
                    return specifier

        new_specifier = Struct(struct_name)
        self.__specifiers.append(new_specifier)
        return self.__specifiers[-1]

    def struct_exists(self, struct_name: str) -> bool:
        for specifier in self.__specifiers:
            if type(specifier) is Struct:
                if specifier.equals(struct_name):
                    return True

        return False

    def get_specifiers(self):
        return self.__specifiers
