from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class RecordDecl(DeclType):
    def __init__(self, struct_name: str):
        self.__struct_name = struct_name
        super().__init__("RecordDecl")

    def get_struct_name(self):
        return self.__struct_name
