from DeclParser.DeclType import DeclType

class FunctionDecl(DeclType):
    def __init__(self, fn_name: str, ret_type: str):
        self.__fn_name = fn_name
        self.__ret_type = ret_type
        super().__init__("FunctionDecl")

    def get_fn_name(self):
        return self.__fn_name
    
    def get_ret_type(self):
        return self.__ret_type