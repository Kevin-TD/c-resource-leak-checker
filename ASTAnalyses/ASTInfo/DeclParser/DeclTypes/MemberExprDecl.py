from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class MemberExprDecl(DeclType):
    def __init__(self, field_specified: str):
        self.__field_specified = field_specified
        super().__init__("MemberExprDecl")

    def get_field_specified(self):
        return self.__field_specified
