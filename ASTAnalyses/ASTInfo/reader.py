import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTInfo.AnnoStructure.AnnotationManager import *
from ASTAnalyses.ASTInfo.Specifiers.FunctionStructure.Function import *
from ASTAnalyses.ASTInfo.Specifiers.StructStructure.Struct import *
from ASTAnalyses.ASTInfo.Specifiers.SpecifierManager import *
from ASTAnalyses.ASTInfo.StructVariables.StructVarManager import *
from ASTAnalyses.ASTInfo.Debug import *

# (build dir)
# python3 ../ASTAnalyses/ASTInfo/reader.py testASTOutput.txt

'''
syntax description 

There are 4 types of declaration: FUNCTION, STRUCT, STRUCT_VARIABLE, and ANNOTATION. An asterik is put before the name to indicate that it's a declaration; e.g., *FUNCTION. 

A function declaration *FUNCTION must be followwed by @NAME (string), @RETURN_TYPE (string), and @PARAMETERS [list of strings]

A struct declaration *STRUCT must be followed by @NAME (string), @FIELDS [list of strings] and @TYPEDEFS [list of strings]

For *STRUCT_VARIABLE, it has @NAME (string) and @TYPE (string)

For *ANNOTATION, it has @ANNO_TYPE (string), @TARGET <string>, and @METHODS [list of strings]
'''


class ASTReader:
    """
    Reads AST information in and gets the functions, structs, struct vars, and annotations into easier-to-handle
    classes (Function, Struct, StructVar, and Annotation)
    """

    def __init__(self, file_to_read):
        self.__file_stream = open(file_to_read, "r")
        self.__functions: list[Function] = []
        self.__structs: list[Struct] = []
        self.__struct_vars: list[StructVar] = []
        self.__annotations: list[Annotation] = []

        char = self.__file_stream.read(1)
        while char:
            if char == "*":
                declaration_type = self.__collect_until("\n")
                if declaration_type == "FUNCTION":

                    self.__collect_and_validate(" ", "@NAME")
                    self.__skip("(")
                    name_data = self.__collect_until(")")
                    self.__skip("\n")

                    self.__collect_and_validate(" ", "@RETURN_TYPE")
                    self.__skip("(")
                    return_type_data = self.__collect_until(")")
                    self.__skip("\n")

                    created_function = Function(name_data, return_type_data)

                    self.__collect_and_validate(" ", "@PARAMETERS")
                    self.__skip("[")
                    parameters_data = self.__collect_until("]").split(",")

                    for (i, param) in enumerate(parameters_data):
                        if param == "":
                            continue

                        logout(
                            f"FUNC: {name_data}, {return_type_data}, {i}, {param}")
                        created_function.add_parameter(Parameter(i, param))

                    self.__functions.append(created_function)

                elif declaration_type == "STRUCT":
                    self.__collect_and_validate(" ", "@NAME")
                    self.__skip("(")
                    name_data = self.__collect_until(")")
                    self.__skip("\n")

                    created_struct = Struct(name_data)

                    self.__collect_and_validate(" ", "@FIELDS")
                    self.__skip("[")
                    fields_data = self.__collect_until("]").split(",")
                    self.__skip("\n")

                    for (i, field) in enumerate(fields_data):
                        if field == "":
                            continue

                        logout(
                            f"STRUCT: {name_data}, {return_type_data}, {i}, {field}")
                        created_struct.add_field(Field(field, i))

                    self.__collect_and_validate(" ", "@TYPEDEFS")
                    self.__skip("[")
                    typedefs_data = self.__collect_until("]").split(",")

                    for (i, typedef) in enumerate(typedefs_data):
                        if typedef == "":
                            continue

                        logout(
                            f"TYPEDEF: {typedef}")
                        created_struct.add_typedef(typedef)

                    self.__structs.append(created_struct)

                elif declaration_type == "STRUCT_VARIABLE":
                    self.__collect_and_validate(" ", "@NAME")
                    self.__skip("(")
                    name_data = self.__collect_until(")")
                    self.__skip("\n")

                    self.__collect_and_validate(" ", "@TYPE")
                    self.__skip("(")
                    type_data = self.__collect_until(")")

                    created_struct_var = StructVar(name_data, type_data)
                    logout(f"STRUCT_VAR: {name_data}, {type_data}")

                    self.__struct_vars.append(created_struct_var)

                elif declaration_type == "ANNOTATION":
                    self.__collect_and_validate(" ", "@ANNO_TYPE")
                    self.__skip("(")
                    anno_type_data = self.__collect_until(")")
                    self.__skip("\n")

                    self.__collect_and_validate(" ", "@TARGET")
                    self.__skip("<")
                    target_data = self.__collect_until(">")
                    self.__skip("\n")

                    self.__collect_and_validate(" ", "@METHODS")
                    self.__skip("[")
                    methods_data = self.__collect_until("]").split(",")
                    methods_str = ""

                    for (i, method) in enumerate(methods_data):
                        methods_str += method
                        if (i != len(methods_data) - 1):
                            methods_str += ", "

                    created_annotation = Annotation(
                        anno_type_data, target_data, methods_str)
                    logout(
                        f"ANNOTATION: {anno_type_data}, {target_data}, {methods_str}")

                    self.__annotations.append(created_annotation)

            char = self.__file_stream.read(1)

        self.__file_stream.close()

    def __skip(self, char):
        """
        Reads current character and errors if what is read is not equal to char
        """
        res = self.__file_stream.read(1)

        if res != char:
            raise ValueError(f"{char} not skipped but {res}")

    def __collect_until(self, char):
        """
        Reads file stream until char is hit 
        """

        res = ""
        read_char = self.__file_stream.read(1)

        while (read_char != char):
            res += read_char
            read_char = self.__file_stream.read(1)

        return res

    def __collect_and_validate(self, char, equals_to):
        """
        Reads file stream until char is hit. Throws and error if what was read does not equal equals_to.
        """
        res = ""
        read_char = self.__file_stream.read(1)

        while (read_char != char):
            res += read_char
            read_char = self.__file_stream.read(1)

        if res != equals_to:
            raise SyntaxError(f"Expected {equals_to} but received {res}")

    def get_functions(self):
        return self.__functions

    def get_structs(self):
        return self.__structs

    def get_struct_vars(self):
        return self.__struct_vars

    def get_annotations(self):
        return self.__annotations
