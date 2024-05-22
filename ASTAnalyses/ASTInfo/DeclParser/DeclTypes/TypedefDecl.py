from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class TypedefDecl(DeclType):
    def __init__(self, original_type_name: str, alias_name: str):
        super().__init__("TypedefDecl")
        self.__original_type_name = original_type_name
        self.__alias_name = alias_name

    def get_original_type_name(self):
        return self.__original_type_name

    def get_alias_name(self):
        return self.__alias_name
