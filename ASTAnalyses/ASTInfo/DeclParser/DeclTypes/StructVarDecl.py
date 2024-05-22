from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType


class StructVarDecl(DeclType):
    # note: in AST this is just a VarDecl but this type exists specifically for VarDecls that specify a struct

    def __init__(self, struct_var_name, struct_type_name):
        super().__init__("StructVarDecl")
        self.__struct_var_name = struct_var_name
        self.__struct_type_name = struct_type_name

    def get_struct_var_name(self):
        return self.__struct_var_name

    def get_struct_type_name(self):
        return self.__struct_type_name
