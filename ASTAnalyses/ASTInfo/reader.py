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

        if declaration_type == "FUNCTION":  
            name_decl = collect_until(ast_info, " ")

            if name_decl == "@NAME":
                name_data = collect_until(ast_info, ")")[1:]
            else: 
                raise SyntaxError("Expected @NAME decl after *FUNCTION")
            
            return_type_decl = collect_until(ast_info, " ")[1:]

            if return_type_decl == "@RETURN_TYPE":
                return_type_data = collect_until(ast_info, ")")[1:]

            created_function = Function(name_data, return_type_data)
            
            # TODO: get changes from draft pull request into this one. manual copy paste
            # TODO: more errors

            parameters_decl = collect_until(ast_info, " ")[1:]

            if parameters_decl == "@PARAMETERS":
                parameters_data = collect_until(ast_info, "]")[1:].split(",")
                for (i, param) in enumerate(parameters_data):
                    if param == "":
                        continue
                    
                    logout(f"FUNC: {name_data}, {return_type_data}, {i}, {param}")
                    created_function.add_parameter(Parameter(i, param))
            
        

        elif declaration_type == "STRUCT":
            name_decl = collect_until(ast_info, " ")

            if name_decl == "@NAME":
                name_data = collect_until(ast_info, ")")[1:]
            
            created_struct = Struct(name_data)
            
            fields_decl = collect_until(ast_info, " ")[1:]

            if fields_decl == "@FIELDS":
                fields_data = collect_until(ast_info, "]")[1:].split(",")

                for (i, field) in enumerate(fields_data):
                    if field == "":
                        continue
                    
                    logout(f"STRUCT: {name_data}, {return_type_data}, {i}, {field}")
                    created_struct.add_field(Field(field, i))
        
        elif declaration_type == "STRUCT_VARIABLE":
            name_decl = collect_until(ast_info, " ")

            if name_decl == "@NAME":
                name_data = collect_until(ast_info, ")")[1:]
            
            type_decl = collect_until(ast_info, " ")[1:]

            if type_decl == "@TYPE":
                type_data = collect_until(ast_info, ")")[1:]
            
            created_struct_var = StructVar(name_data, type_data)
            logout(f"STRUCT_VAR: {name_data}, {type_data}")
        
        elif declaration_type == "ANNOTATION":
            string_decl = collect_until(ast_info, " ")

            if string_decl == "@STRING":
                string_data = collect_until(ast_info, "\n")[1:]
                string_data = string_data[:len(string_data) - 1]
            
            # example annotation
            # Calls target = FUNCTION(does_something).PARAM(0).FIELD(0) methods = free
            # assumed well-formed
            anno_chunks = string_data.split(" ")
            # TODO: instead of annotation being the whole thing, make it a @ANNO_TYPE, @TARGET, @METHODS [...]

            Annotation()
            
    
    char = ast_info.read(1)

ast_info.close()