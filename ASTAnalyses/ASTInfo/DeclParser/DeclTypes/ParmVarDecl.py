from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class ParmVarDecl(DeclType):
    def __init__(self, parm_index: int, parm_type: str):
        self.__parm_index = parm_index
        self.__parm_type = parm_type
        super().__init__("ParmVarDecl")

    def get_parm_index(self):
        return self.__parm_index

    def get_parm_type(self):
        return self.__parm_type
