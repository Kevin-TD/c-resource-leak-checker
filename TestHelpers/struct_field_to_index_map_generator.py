#! python3 struct_field_to_index_map_generator.py <ast txt file> <output for map file> 
# generate AST text with: clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics <c file> > <ast txt file>

# example (run in build dir): 
# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/simple_ptr_test/index.c > ast.txt ; python3 ../TestHelpers/
# struct_field_to_index_map_generator.py ast.txt map.txt

# finds every variable whose type is a struct and creates a map of it and its field names to it and its field indices. 
# example: 
'''
struct my_struct {
 char* x;
 char* y;
}

int main() {
    struct my_struct A;
    struct my_struct B;
}
'''
# this C code will generate the following map: 
'''
[A.x] => [A.0]
[A.y] => [A.1]
[B.x] => [B.0]
[B.y] => [B.1]
'''

import sys 

DEBUG = False

def logout(x: str):
    if (DEBUG):
        print(x)
    
class Field:
    def __init__(self, field_name: str, field_index: int):
        self.field_name = field_name
        self.field_index = field_index

class Struct:
    def __init__(self, name: str):
        self.name = name
        self.fields: list[Field] = []

    def get_name(self) -> str: 
        return self.name
    
    def add_field(self, field: Field) -> None:
        self.fields.append(field)
    

class StructsManager:
    def __init__(self):
        self.structs: list[Struct] = []
    
    # if struct is not found, a new one is returned with name struct_name if error_if_not_found is false
    def get_struct(self, struct_name: str, error_if_not_found: bool = False) -> Struct:
        for struct in self.structs:
            if struct.get_name() == struct_name:
                return struct
        
        if error_if_not_found:
            raise ValueError("struct not found")

        new_struct = Struct(struct_name)
        self.structs.append(new_struct)
        return self.structs[-1] 
    
    # if there already exists a struct of name struct_name, that struct is returned. 
    # otherwise, the struct is added and returned. 
    def add_struct(self, struct_name: str) -> bool:
        for struct in self.structs:
            if struct.get_name() == struct_name: 
                return struct
        
        new_struct = Struct(struct_name)
        self.structs.append(new_struct)
        return self.structs[-1] 

class StructVar:
    def __init__(self, var_name: str, type_name: str):
        self.var_name = var_name
        self.type_name = type_name
    
    def get_var_name(self):
        return self.var_name

    def get_type_name(self):
        return self.type_name

class StructVarManager:
    def __init__(self):
        self.struct_vars: list[StructVar] = []
    
    # if there already exists a var of name var_name, that struct is returned. 
    # otherwise, the var is added and returned. 
    def add_struct_var(self, var_name: str, type_name: str) -> bool:
        for struct_var in self.struct_vars:
            if struct_var.get_var_name() == var_name: 
                return struct_var
        
        new_struct_var = StructVar(var_name, type_name)
        self.struct_vars.append(new_struct_var)
        return self.struct_vars[-1] 
    
def add_struct(record_decl: str, structs_manager: StructsManager):
    record_decl_chunks = record_decl.split(" ")
    struct_name = record_decl_chunks[len(record_decl_chunks) - 2]

    return structs_manager.add_struct(struct_name)


def add_field(field_decl: str, field_index: int, struct: Struct):
    if type(struct) != Struct: 
        return

    start_index = field_decl.find("'") - 2
    cur_char = field_decl[start_index]
    field_name = ""

    while (cur_char != " "):
        field_name = cur_char + field_name

        start_index -= 1
        cur_char = field_decl[start_index]
    

    field = Field(field_name, field_index)
    struct.add_field(field)

def add_struct_var(var_decl: str, struct_var_manager: StructVarManager):
    var_decl_chunks = var_decl.split(" ")
    
    var_name = var_decl_chunks[7]

    struct_name = ""
    start_index = var_decl.find("'") + 1
    cur_char = var_decl[start_index]

    while (cur_char != "'"):
        struct_name += cur_char 

        start_index += 1
        cur_char = var_decl[start_index]
    
    struct_name_split = struct_name.split(" ")
    if struct_name_split[1] != "":
        struct_name = struct_name_split[1]
    else:
        struct_name = struct_name_split[0]

    logout(var_decl)
    logout(f"parsed var_decl var name = {var_name} struct name = {struct_name}")

    struct_var_manager.add_struct_var(var_name, struct_name)


    
file_to_read = sys.argv[1]
output_file = sys.argv[2]

with open(file_to_read) as ast:
    cur_struct = None 
    cur_top_decl = None  #FunctionDecl or RecordDecl (strings)
    cur_mid_decl = None #ParmVarDecl or FieldDecl (strings)
    field_index = None

    ast_lines = ast.readlines()
    structs_manager = StructsManager() 
    struct_var_manager = StructVarManager()
    
    
    for expr in ast_lines: 
        expr = expr.strip() 

        # gets decl 
        start_decl_index = expr.find(next(filter(str.isalpha, expr))) # finds index of first letter
        decl = ""
        cur_decl_char = expr[start_decl_index]

        try: 
            while (cur_decl_char != " "):
                decl += cur_decl_char
                start_decl_index += 1
                cur_decl_char = expr[start_decl_index]
        except IndexError:
            # might be a null stement (looks like "<<<NULL>>>" in AST)
            field_index = None
            continue 

        if "FunctionDecl" == decl:
            cur_top_decl = expr
            field_index = None 
            
        elif "RecordDecl" == decl:
            cur_top_decl = expr
            field_index = None 
            cur_struct = add_struct(cur_top_decl, structs_manager)

        elif "ParmVarDecl" == decl: 
            cur_mid_decl = expr
            field_index = None 


        elif "FieldDecl" == decl:
            cur_mid_decl = expr 
            
            if field_index is None: 
                field_index = 0
            else:
                field_index += 1

            add_field(cur_mid_decl, field_index, cur_struct)
        
            
        elif "AnnotateAttr" == decl:
            if "|-" not in expr: 
                cur_mid_decl = None

        else:
            if "VarDecl" == decl and "struct" in expr and "used" in expr:
                add_struct_var(expr, struct_var_manager)

            field_index = None

    with open(output_file, "w") as output:
        for i in range(len(struct_var_manager.struct_vars)):
            struct_var = struct_var_manager.struct_vars[i]
            
            found_struct = structs_manager.get_struct(struct_var.get_type_name(), True)

            for j in range(len(found_struct.fields)):
                field = found_struct.fields[j]
                output.write(f"{struct_var.get_var_name()}.{field.field_name}={struct_var.get_var_name()}.{field.field_index}\n")

                logout(f"wrote: {struct_var.get_var_name()}.{field.field_name}={struct_var.get_var_name()}.{field.field_index}")