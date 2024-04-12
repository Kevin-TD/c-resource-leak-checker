from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class FieldDecl(DeclType):
    def __init__(self, field_name: str, field_index: int):
        self.__field_name = field_name
        self.__field_index = field_index
        super().__init__("FieldDecl")

    def get_field_name(self):
        return self.__field_name

    def get_field_index(self):
        return self.__field_index
