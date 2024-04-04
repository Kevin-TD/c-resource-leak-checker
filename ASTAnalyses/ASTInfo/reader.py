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


    

with open(file_to_read) as ast_info:
    '''
    syntax description 

    There are 4 types of declaration: FUNCTION, STRUCT, STRUCT_VARIABLE, and ANNOTATION. An asterik is put before the name to indicate that it's a declaration; e.g., *FUNCTION. 

    A function declaration *FUNCTION must be followwed by @NAME(string), @RETURN_TYPE(string), and @PARAMETERS[list of strings]

    A struct declaration *STRUCT must be followed by @NAME(string)

    For *STRUCT_VARIABLE, it has @NAME(string), @TYPE(string)

    For *ANNOTATION, it has @STRING(string)
    
    
    ''' 
    char = ast_info.read(1)

    if char == "*":
        declaration_type = collect_until(ast_info, "\n")
        print(f"'{declaration_type}'")

        if declaration_type == "FUNCTION":  
            attribute_type = collect_until(ast_info, " ")
            print(f"'{attribute_type}'")

            if attribute_type == "@NAME":
                name_data = collect_until(ast_info, ")")[1:]
                print(f"'{name_data}'")
            elif attribute_type == "@RETURN_TYPE":
                return_type_data = collect_until(ast_info, ")")[1:]
                print(f"'{return_type_data}'")
            elif attribute_type == "@PARAMETERS":
                paramters_data = collect_until(ast_info, "]")[1:]
                print(f"'{paramters_data}'")