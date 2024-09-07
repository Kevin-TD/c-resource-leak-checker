from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class ReturnStmtDecl(DeclType):
    def __init__(self, line_number: int):
        self.__line_number = line_number
        super().__init__("ReturnStmtDecl")

    def get_line_number(self):
        return self.__line_number
