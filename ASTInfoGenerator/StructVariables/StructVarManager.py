from ASTInfoGenerator.StructVariables.StructVar import StructVar


class StructVarManager:
    """Wrapper for a collection of struct variables
    """

    def __init__(self):
        self.struct_vars: list[StructVar] = []

    # if there already exists a var of name var_name, that struct var is returned.
    # otherwise, the var is added and returned.
    def add_struct_var(self, var_name: str, type_name: str) -> bool:
        for struct_var in self.struct_vars:
            if struct_var.get_var_name() == var_name:
                return struct_var

        new_struct_var = StructVar(var_name, type_name)
        self.struct_vars.append(new_struct_var)
        return self.struct_vars[-1]
