from ASTAnalyses.ASTInfo.Specifiers.FunctionStructure.Parameter import Parameter
from ASTAnalyses.ASTInfo.Specifiers.Specifier import Specifier


class Function(Specifier):
    """Represents a function in the C code
    """

    def __init__(self, name: str, return_type: str):
        super().__init__(name)
        self.__return_type = return_type
        self.__parameters: list[Parameter] = []

    def get_return_type(self) -> str:
        return self.__return_type

    def get_parameters(self) -> "list[Parameter]":
        """returns copy of parameters
        """

        return self.__parameters.copy()

    def set_return_type(self, return_type: str) -> None:
        self.return_type = return_type

    def add_parameter(self, parameter: Parameter) -> None:
        for param in self.__parameters:
            if param.get_index() == parameter.get_index():
                return

        self.__parameters.append(parameter)

    def equals(self, function_name: str):
        """returns true iff function_name equals Function's name
        """

        return self._name == function_name
