from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class DeclRefExprDecl(DeclType):
    def __init__(self, var_name: str):
        self.__var_name = var_name
        super().__init__("DeclRefExprDecl")

    def get_var_name(self):
        return self.__var_name
