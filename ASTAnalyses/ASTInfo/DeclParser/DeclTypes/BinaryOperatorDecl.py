from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class BinaryOperatorDecl(DeclType):
    __VALID_OPERATOR_TYPES = ["="]

    def __init__(self, operator_type: str, line_number: int):
        self.__operator_type = operator_type
        self.__line_number = line_number

        if not BinaryOperatorDecl.is_valid_operator_type(operator_type):
            raise ValueError(
                f"Operator '{operator_type}' is not a valid operator type")

        super().__init__("BinaryOperatorDecl")

    def get_operator_type(self):
        return self.__operator_type

    def get_line_number(self):
        return self.__line_number

    @staticmethod
    def is_valid_operator_type(operator_type: str):
        return operator_type in BinaryOperatorDecl.__VALID_OPERATOR_TYPES
