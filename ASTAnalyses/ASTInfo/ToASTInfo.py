import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTInfo.AnnoStructure.AnnotationManager import *
from ASTAnalyses.ASTInfo.Specifiers.FunctionStructure.Function import *
from ASTAnalyses.ASTInfo.Specifiers.StructStructure.Struct import *
from ASTAnalyses.ASTInfo.Specifiers.SpecifierManager import *
from ASTAnalyses.ASTInfo.StructVariables.StructVarManager import *
from ASTAnalyses.ASTInfo.Debug import *
from ASTAnalyses.ASTInfo.ast_info_fmt_specifier import *
from ASTAnalyses.ASTInfo.DeclParser.DeclParser import *


class ASTConverter():

    def convert_function_spec_to_ast_info(struct_spec: Function) -> str:
        output_str = f"*FUNCTION\n"
        output_str += f"@NAME ({struct_spec.get_name()})\n"
        output_str += f"@RETURN_TYPE ({struct_spec.get_return_type().strip()})\n"

        param_str = ""

        for (i, parameter) in enumerate(struct_spec.get_parameters()):
            param_str += parameter.get_param_type()
            if (i != len(struct_spec.get_parameters()) - 1):
                param_str += ","

        if param_str != "":
            output_str += f"@PARAMETERS [{param_str.strip()}]\n"
        else:
            output_str += "@PARAMETERS []\n"

        output_str += "\n"

        return output_str

    def convert_ast_info_func_to_function_spec(self, file_stream) -> Function:
        char = file_stream.read(1)
        if char == "*":
            declaration_type = self.__collect_until("\n")
            if declaration_type == "FUNCTION":

                self.__collect_and_validate(file_stream, " ", "@NAME")
                self.__skip(file_stream, "(")
                name_data = self.__collect_until(file_stream, ")")
                self.__skip("\n")

                self.__collect_and_validate(file_stream, " ", "@RETURN_TYPE")
                self.__skip(file_stream, "(")
                return_type_data = self.__collect_until(file_stream, ")")
                self.__skip(file_stream, "\n")

                created_function = Function(name_data, return_type_data)

                self.__collect_and_validate(file_stream, " ", "@PARAMETERS")
                self.__skip(file_stream, "[")
                parameters_data = self.__collect_until(
                    file_stream, "]").split(",")

                for (i, param) in enumerate(parameters_data):
                    if param == "":
                        continue

                    logout(
                        f"FUNC: {name_data}, {return_type_data}, {i}, {param}")
                    created_function.add_parameter(Parameter(i, param))

                self.__functions.append(created_function)

    def __skip(self, file_stream, char):
        """
        Reads current character and errors if what is read is not equal to char
        """
        res = file_stream.read(1)

        if res != char:
            raise ValueError(f"{char} not skipped but {res}")

    def __collect_until(self, file_stream, char):
        """
        Reads file stream until char is hit 
        """

        res = ""
        read_char = file_stream.read(1)

        while (read_char != char):
            res += read_char
            read_char = file_stream.read(1)

        return res

    def __collect_and_validate(self, file_stream, char, equals_to):
        """
        Reads file stream until char is hit. Throws and error if what was read does not equal equals_to.
        """
        res = ""
        read_char = file_stream.read(1)

        while (read_char != char):
            res += read_char
            read_char = file_stream.read(1)

        if res != equals_to:
            raise SyntaxError(f"Expected {equals_to} but received {res}")
