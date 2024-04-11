import sys
sys.path.insert(0, '..')

from AnnoStructure.AnnotationManager import *
from Specifiers.FunctionStructure.Function import *
from Specifiers.StructStructure.Struct import *
from Specifiers.SpecifierManager import *
from StructVariables.StructVarManager import *
from ASTAnalyses.ASTInfo.Debug import *

# (build dir)
# python3 ../ASTAnalyses/ASTInfo/reader.py testASTOutput.txt

file_to_read = sys.argv[1]


def collect_until(text_io_wrapper, char):
    res = ""
    read_char = text_io_wrapper.read(1)

    while (read_char != char):
        res += read_char
        read_char = text_io_wrapper.read(1)
    
    return res

    
'''
syntax description 

There are 4 types of declaration: FUNCTION, STRUCT, STRUCT_VARIABLE, and ANNOTATION. An asterik is put before the name to indicate that it's a declaration; e.g., *FUNCTION. 

A function declaration *FUNCTION must be followwed by @NAME(string), @RETURN_TYPE(string), and @PARAMETERS[list of strings]

A struct declaration *STRUCT must be followed by @NAME(string)

For *STRUCT_VARIABLE, it has @NAME(string), @TYPE(string)

For *ANNOTATION, it has @STRING(string)
''' 


ast_info = open(file_to_read, "r")

char = ast_info.read(1)

while char:

    if char == "*":
        declaration_type = collect_until(ast_info, "\n")
        logout(f"'{declaration_type}'")

        if declaration_type == "FUNCTION":  
            name_decl = collect_until(ast_info, " ")
            logout(f"'{name_decl}'")

            if name_decl == "@NAME":
                name_data = collect_until(ast_info, ")")[1:]
                logout(f"'{name_data}'")
            else: 
                raise SyntaxError("Expected @NAME decl after *FUNCTION")
            
            return_type_decl = collect_until(ast_info, " ")[1:]
            logout(f"'{return_type_decl}'")

            if return_type_decl == "@RETURN_TYPE":
                return_type_data = collect_until(ast_info, ")")[1:]
                logout(f"'{return_type_data}'")
            
            created_function = Function(name_data, return_type_data)
            
            # TODO: get changes from draft pull request into this one. manual copy paste
            # TODO: int main function takes no args. make it so when arg list is empty it's just PARAMETERS []. same with STRUCT @FIELDS
            # TODO: more errors

            parameters_decl = collect_until(ast_info, " ")[1:]
            logout(f"'{parameters_decl}'")

            if parameters_decl == "@PARAMETERS":
                parameters_data = collect_until(ast_info, "]")[1:].split(",")
                logout(f"'{parameters_data}'")
                for (i, param) in enumerate(parameters_data):
                    if param == "":
                        continue

                    created_function.add_parameter(Parameter(i, param))
            
        

        elif declaration_type == "STRUCT":
            name_decl = collect_until(ast_info, " ")
            logout(f"'{name_decl}'")

            if name_decl == "@NAME":
                name_data = collect_until(ast_info, ")")[1:]
                logout(f"'{name_data}'")
            
            fields_decl = collect_until(ast_info, " ")[1:]
            logout(f"'{fields_decl}'")

            if fields_decl == "@FIELDS":
                fields_data = collect_until(ast_info, "]")[1:].split(",")
                logout(f"'{fields_data}'")
        
        elif declaration_type == "STRUCT_VARIABLE":
            name_decl = collect_until(ast_info, " ")
            logout(f"'{name_decl}'")

            if name_decl == "@NAME":
                name_data = collect_until(ast_info, ")")[1:]
                logout(f"'{name_data}'")
            
            type_decl = collect_until(ast_info, " ")[1:]
            logout(f"'{type_decl}'")

            if type_decl == "@TYPE":
                type_data = collect_until(ast_info, ")")[1:]
                logout(f"'{type_data}'")
        
        elif declaration_type == "ANNOTATION":
            string_decl = collect_until(ast_info, " ")
            logout(f"'{string_decl}'")

            if string_decl == "@STRING":
                string_data = collect_until(ast_info, "\n")[1:]
                string_data = string_data[:len(string_data) - 1]
                logout(f"'{string_data}'")
            
        
        logout("")
    
    char = ast_info.read(1)

ast_info.close()