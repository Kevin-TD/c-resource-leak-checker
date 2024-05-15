import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTInfo.AnnoStructure.AnnotationManager import *
from ASTAnalyses.ASTInfo.Specifiers.FunctionStructure.Function import *
from ASTAnalyses.ASTInfo.Specifiers.StructStructure.Struct import *
from ASTAnalyses.ASTInfo.Specifiers.SpecifierManager import *
from ASTAnalyses.ASTInfo.StructVariables.StructVarManager import *
from ASTAnalyses.ASTInfo.Debug import *
from ASTAnalyses.ASTInfo.ast_info_tokens import *

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
            if char == AST_INFO_TOKENS['DeclStart']:
                declaration_type = self.__collect_until(
                    AST_INFO_TOKENS['EndOfLineChar'])
                if declaration_type == AST_INFO_TOKENS['FunctionDecl']:

                    self.__collect_and_validate(
                        AST_INFO_TOKENS['AttrValueSeparation'], AST_INFO_TOKENS['NameAttr'])
                    self.__skip(AST_INFO_TOKENS['ValueOpenChar'])
                    name_data = self.__collect_until(
                        AST_INFO_TOKENS['ValueCloseChar'])
                    self.__skip(AST_INFO_TOKENS['EndOfLineChar'])

                    self.__collect_and_validate(
                        AST_INFO_TOKENS['AttrValueSeparation'], AST_INFO_TOKENS['ReturnTypeAttr'])
                    self.__skip(AST_INFO_TOKENS['ValueOpenChar'])
                    return_type_data = self.__collect_until(
                        AST_INFO_TOKENS['ValueCloseChar'])
                    self.__skip(AST_INFO_TOKENS['EndOfLineChar'])

                    created_function = Function(name_data, return_type_data)

                    self.__collect_and_validate(
                        AST_INFO_TOKENS['AttrValueSeparation'], AST_INFO_TOKENS['ParamsAttr'])
                    self.__skip(AST_INFO_TOKENS['ValuesOpenChar'])
                    parameters_data = self.__collect_until(
                        AST_INFO_TOKENS['ValuesCloseChar']).split(AST_INFO_TOKENS['ValuesDelimeter'])

                    for (i, param) in enumerate(parameters_data):
                        if param == "":
                            continue

                        logout(
                            f"FUNC: {name_data}, {return_type_data}, {i}, {param}")
                        created_function.add_parameter(Parameter(i, param))

                    self.__functions.append(created_function)

                elif declaration_type == AST_INFO_TOKENS["StructDecl"]:
                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["NameAttr"])
                    self.__skip(AST_INFO_TOKENS["ValueOpenChar"])
                    name_data = self.__collect_until(
                        AST_INFO_TOKENS["ValueCloseChar"])
                    self.__skip(AST_INFO_TOKENS["EndOfLineChar"])

                    created_struct = Struct(name_data)

                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["FieldsAttr"])
                    self.__skip(AST_INFO_TOKENS["ValuesOpenChar"])
                    fields_data = self.__collect_until(AST_INFO_TOKENS["ValuesCloseChar"]).split(
                        AST_INFO_TOKENS["ValuesDelimeter"])
                    self.__skip(AST_INFO_TOKENS["EndOfLineChar"])

                    for (i, field) in enumerate(fields_data):
                        if field == "":
                            continue

                        logout(
                            f"STRUCT: {name_data}, {return_type_data}, {i}, {field}")
                        created_struct.add_field(Field(field, i))

                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["TypedefsAttr"])
                    self.__skip(AST_INFO_TOKENS["ValuesOpenChar"])
                    typedefs_data = self.__collect_until(AST_INFO_TOKENS["ValuesCloseChar"]).split(
                        AST_INFO_TOKENS["ValuesDelimeter"])

                    for (i, typedef) in enumerate(typedefs_data):
                        if typedef == "":
                            continue

                        logout(
                            f"TYPEDEF: {typedef}")
                        created_struct.add_typedef(typedef)

                    self.__structs.append(created_struct)

                elif declaration_type == AST_INFO_TOKENS["StructVarDecl"]:
                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["NameAttr"])
                    self.__skip(AST_INFO_TOKENS["ValueOpenChar"])
                    name_data = self.__collect_until(
                        AST_INFO_TOKENS["ValueCloseChar"])
                    self.__skip("\n")

                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["TypeAttr"])
                    self.__skip(AST_INFO_TOKENS["ValueOpenChar"])
                    type_data = self.__collect_until(
                        AST_INFO_TOKENS["ValueCloseChar"])

                    created_struct_var = StructVar(name_data, type_data)
                    logout(f"STRUCT_VAR: {name_data}, {type_data}")

                    self.__struct_vars.append(created_struct_var)

                elif declaration_type == AST_INFO_TOKENS["AnnoDecl"]:
                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["AnnoTypeAttr"])
                    self.__skip(AST_INFO_TOKENS["ValueOpenChar"])
                    anno_type_data = self.__collect_until(
                        AST_INFO_TOKENS["ValueCloseChar"])
                    self.__skip(AST_INFO_TOKENS["EndOfLineChar"])

                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["TargetAttr"])
                    self.__skip(AST_INFO_TOKENS["TargetOpenChar"])
                    target_data = self.__collect_until(
                        AST_INFO_TOKENS["TargetCloseChar"])
                    self.__skip(AST_INFO_TOKENS["EndOfLineChar"])

                    self.__collect_and_validate(
                        AST_INFO_TOKENS["AttrValueSeparation"], AST_INFO_TOKENS["MethodsAttr"])
                    self.__skip(AST_INFO_TOKENS["ValuesOpenChar"])
                    methods_data = self.__collect_until(AST_INFO_TOKENS["ValuesCloseChar"]).split(
                        AST_INFO_TOKENS["ValuesDelimeter"])
                    methods_str = ""

                    # NOTE: do not update section using AST_INFO_TOKENS.
                    # methods_str and Annotation refer to different API
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
