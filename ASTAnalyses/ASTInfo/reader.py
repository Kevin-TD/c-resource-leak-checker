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


def collect_and_validate(text_io_wrapper, char, equals_to):
    res = ""
    read_char = text_io_wrapper.read(1)

    while (read_char != char):
        res += read_char
        read_char = text_io_wrapper.read(1)

    if res != equals_to:
        raise SyntaxError(f"Expected {equals_to} but received {res}")

    return res


def skip(text_io_wrapper, char):
    res = text_io_wrapper.read(1)

    if res != char:
        raise ValueError(f"{char} not skipped but {res}")


'''
syntax description 

There are 4 types of declaration: FUNCTION, STRUCT, STRUCT_VARIABLE, and ANNOTATION. An asterik is put before the name to indicate that it's a declaration; e.g., *FUNCTION. 

A function declaration *FUNCTION must be followwed by @NAME (string), @RETURN_TYPE(string), and @PARAMETERS [list of strings]

A struct declaration *STRUCT must be followed by @NAME(string) and @FIELDS [list of strings]

For *STRUCT_VARIABLE, it has @NAME(string) and @TYPE(string)

For *ANNOTATION, it has @ANNO_TYPE(string), @TARGET<string>, and @METHODS [list of strings]
'''


ast_info = open(file_to_read, "r")

char = ast_info.read(1)

while char:

    if char == "*":
        declaration_type = collect_until(ast_info, "\n")

        if declaration_type == "FUNCTION":

            name_decl = collect_and_validate(ast_info, " ", "@NAME")
            skip(ast_info, "(")
            name_data = collect_until(ast_info, ")")
            skip(ast_info, "\n")

            return_type_decl = collect_and_validate(
                ast_info, " ", "@RETURN_TYPE")
            skip(ast_info, "(")
            return_type_data = collect_until(ast_info, ")")
            skip(ast_info, "\n")

            created_function = Function(name_data, return_type_data)

            parameters_decl = collect_and_validate(
                ast_info, " ", "@PARAMETERS")
            skip(ast_info, "[")
            parameters_data = collect_until(ast_info, "]").split(",")

            for (i, param) in enumerate(parameters_data):
                if param == "":
                    continue

                logout(
                    f"FUNC: {name_data}, {return_type_data}, {i}, {param}")
                created_function.add_parameter(Parameter(i, param))

        elif declaration_type == "STRUCT":
            name_decl = collect_and_validate(ast_info, " ", "@NAME")
            skip(ast_info, "(")
            name_data = collect_until(ast_info, ")")
            skip(ast_info, "\n")

            created_struct = Struct(name_data)

            fields_decl = collect_and_validate(ast_info, " ", "@FIELDS")
            skip(ast_info, "[")
            fields_data = collect_until(ast_info, "]").split(",")

            for (i, field) in enumerate(fields_data):
                if field == "":
                    continue

                logout(
                    f"STRUCT: {name_data}, {return_type_data}, {i}, {field}")
                created_struct.add_field(Field(field, i))

        elif declaration_type == "STRUCT_VARIABLE":
            name_decl = collect_and_validate(ast_info, " ", "@NAME")
            skip(ast_info, "(")
            name_data = collect_until(ast_info, ")")
            skip(ast_info, "\n")

            type_decl = collect_and_validate(ast_info, " ", "@TYPE")
            skip(ast_info, "(")
            type_data = collect_until(ast_info, ")")

            created_struct_var = StructVar(name_data, type_data)
            logout(f"STRUCT_VAR: {name_data}, {type_data}")

        elif declaration_type == "ANNOTATION":
            anno_type_decl = collect_and_validate(ast_info, " ", "@ANNO_TYPE")
            skip(ast_info, "(")
            anno_type_data = collect_until(ast_info, ")")
            skip(ast_info, "\n")

            target_decl = collect_and_validate(ast_info, " ", "@TARGET")
            skip(ast_info, "<")
            target_data = collect_until(ast_info, ">")
            skip(ast_info, "\n")

            methods_decl = collect_and_validate(ast_info, " ", "@METHODS")
            skip(ast_info, "[")
            methods_data = collect_until(ast_info, "]").split(",")
            methods_str = ""

            for (i, method) in enumerate(methods_data):
                methods_str += method
                if (i != len(methods_data) - 1):
                    methods_str += ","

            created_annotation = Annotation(
                anno_type_data, target_data, methods_str)
            logout(
                f"ANNOTATION: {anno_type_data}, {target_data}, {methods_str}")

    char = ast_info.read(1)

ast_info.close()
