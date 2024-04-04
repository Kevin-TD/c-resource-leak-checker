from DeclParser.DeclType import DeclType
from ASTInfoGenerator.Debug import *
from ASTInfoGenerator.Specifiers.SpecifierManager import *

from ASTInfoGenerator.DeclParser.DeclTypes.FunctionDecl import *
from ASTInfoGenerator.DeclParser.DeclTypes.RecordDecl import *
from ASTInfoGenerator.DeclParser.DeclTypes.ParmVarDecl import *
from ASTInfoGenerator.DeclParser.DeclTypes.TypedefDecl import *
from ASTInfoGenerator.DeclParser.DeclTypes.FieldDecl import *
from ASTInfoGenerator.DeclParser.DeclTypes.StructVarDecl import *
from ASTInfoGenerator.DeclParser.DeclTypes.AnnotateAttr import *


class DeclParser:
    def __init__(self):
        self.__param_index = None
        self.__field_index = None

    def _parse_function_decl(self, function_decl: str) -> Specifier:
        # Assumption: functions that are extern are not relevant to our analysis
        if function_decl.endswith("extern"):
            return None

        logout(f"function decl equals {function_decl}")

        quote_index = function_decl.find("'")
        # Assumption: FunctionDecls wrap in quotes ('...') the return type of the function & parameters, and
        # right before the return type is the name of the function.
        # we also assume the function name and return type are separated by a space.
        # e.g., |-FunctionDecl 0x24161a8 <../test/test23/test23.h:27:1, line:28:57> col:5 does_something 'struct my_struct (struct my_struct)'
        #                                                                                 ^func name     ^return type      ^parameters

        start_index = quote_index - 2
        # quote_index - 2 since we will be traversing FunctionDecl
        # backward, starting at the last character of the name of the function,
        # in order to get the full function name

        cur_char = function_decl[start_index]
        function_name = ""

        while (cur_char != " "):
            function_name = cur_char + function_name

            start_index -= 1
            cur_char = function_decl[start_index]

        start_index = quote_index + 1
        # quote_index + 1 since we assume this is where
        # the return type is specified

        cur_char = function_decl[start_index]
        return_type = ""

        while (cur_char != "("):
            # we assume that parameter types are specified within
            # the open-close parentheses
            # e.g., 'int (int)' is a function that returns int with singular parameter int
            # e.g., 'int ()' is a function that returns int with no parameters

            # e.g., 'void (int, int, char *)' is a function that returns void with
            # 3 parameters (in the order as it appears in C code), int, int, and char *

            return_type += cur_char

            start_index += 1
            cur_char = function_decl[start_index]

        logout(
            f"function name = '{function_name}', return type = '{return_type}'")
        return FunctionDecl(function_name, return_type)

    def _parse_record_decl(self, record_decl: str):
        record_decl_chunks = record_decl.split(" ")
        struct_name = record_decl_chunks[len(record_decl_chunks) - 2]

        return RecordDecl(struct_name)

    def _parse_parm_var_decl(self, parm_var_decl: str, param_index: int):
        quote_second_index = parm_var_decl.find(
            "'", parm_var_decl.find("'") + 1)

        param_type = parm_var_decl[
            parm_var_decl.find("'") + 1: quote_second_index
        ]

        # check if param type is formatted like 'typedef_name':'struct_name *'
        # we extract it into just `struct_name *`
        start_of_struct_type_name_index = quote_second_index + 3
        if len(parm_var_decl) > start_of_struct_type_name_index and "*" in parm_var_decl[start_of_struct_type_name_index:]:
            param_type = parm_var_decl[start_of_struct_type_name_index: len(
                parm_var_decl) - 1]
            param_type = param_type[: param_type.find("'")]

        param_type = param_type.replace(" ", "")
        logout(f"param added {param_type} for {parm_var_decl}")

        return ParmVarDecl(param_index, param_type)

    def _parse_typedef_decl(self, typedef_decl: str):
        if not ("referenced" in typedef_decl and "struct" in typedef_decl):
            return None
        original_name = ""
        start_index = typedef_decl.find("'") + 1
        cur_char = typedef_decl[start_index]

        while (cur_char != "'"):
            original_name += cur_char

            start_index += 1
            cur_char = typedef_decl[start_index]

        original_name_split = original_name.split(" ")
        if len(original_name_split) > 1 and original_name_split[1] != "":
            original_name = original_name_split[1]
        else:
            original_name = original_name_split[0]

        typedef_alias_name = ""
        start_index = typedef_decl.find("referenced") + len("referenced") + 1
        cur_char = typedef_decl[start_index]

        while (cur_char != " "):
            typedef_alias_name += cur_char

            start_index += 1
            cur_char = typedef_decl[start_index]

        if original_name != typedef_alias_name:
            return TypedefDecl(original_name, typedef_alias_name)

        return None

    def _parse_field_decl(self, field_decl: str, field_index: int):
        start_index = field_decl.find("'") - 2
        cur_char = field_decl[start_index]
        field_name = ""

        while (cur_char != " "):
            field_name = cur_char + field_name

            start_index -= 1
            cur_char = field_decl[start_index]

        return FieldDecl(field_name, field_index)

    # only checks for vars whose type is struct
    def _parse_var_decl(self, var_decl: str, specifier_manager: SpecifierManager):
        var_decl_chunks = var_decl.split(" ")

        logout(var_decl)
        logout(var_decl_chunks)

        if var_decl_chunks[5] == "used":
            var_name = var_decl_chunks[6]
        elif var_decl_chunks[6] == "used":
            var_name = var_decl_chunks[7]
        else:
            var_name = var_decl_chunks[5]

        struct_name = ""
        start_index = var_decl.find("'") + 1
        cur_char = var_decl[start_index]

        while (cur_char != "'"):
            struct_name += cur_char

            start_index += 1
            cur_char = var_decl[start_index]

        struct_name_split = struct_name.split(" ")

        # check if type is formatted like 'my_struct *' (typedef alias in use)
        # instead of 'struct my_struct *' (no typedef alias in use)
        type_formatted_using_typedef = len(
            struct_name_split) > 1 and "*" in struct_name_split

        if len(struct_name_split) == 2 and "*" in struct_name_split[1]:
            struct_name = struct_name_split[0]
        elif len(struct_name_split) > 1 and struct_name_split[1] != "" and not type_formatted_using_typedef:
            struct_name = struct_name_split[1]
        else:
            struct_name = struct_name_split[0]

        if not specifier_manager.struct_exists(struct_name):
            logout(
                f"did not find for parsed var_decl var name = {var_name} struct name = '{struct_name}'")
            return None

        logout(var_decl)
        logout(
            f"did parsed var_decl var name = {var_name} struct name = '{struct_name}'")

        return StructVarDecl(var_name, struct_name)

    def _parse_anno(self, anno_decl: str, specifier_manager: SpecifierManager, param_index: int, field_index: int):
        spec = specifier_manager.get_specifiers()[-1]

        logout(f"pre is {anno_decl}")

        start_anno_index = anno_decl.find('"')
        end_anno_index = anno_decl.rfind('"')

        anno_decl = anno_decl[start_anno_index + 1: end_anno_index]

        anno_type = anno_decl[
            0: anno_decl.find(" ")
        ]

        anno_methods = anno_decl[anno_decl.rfind(
            "methods = ") + len("methods = ")::]
        logout(f"ANNO METHODS = {anno_methods}")

        known_target = ""

        anno_unfilled_target = ""
        anno_unfilled_target_index = anno_decl.find("_")

        while (anno_decl[anno_unfilled_target_index] != " "):
            anno_unfilled_target += anno_decl[anno_unfilled_target_index]
            anno_unfilled_target_index += 1

        # anno_unfilled_target looks like "_" or "_.FIELD(x)"

        if type(spec) is Struct:
            known_target = f"STRUCT({spec.get_name()}).FIELD({field_index})"

        elif type(spec) is Function:
            known_target = f"FUNCTION({spec.get_name()})"

            # Assumption: if it is not referring to a parameter, it is
            # referring to the function itself
            if param_index is None:
                known_target += ".RETURN"
            else:
                known_target += f".PARAM({param_index})"

        if ".FIELD" in anno_unfilled_target:
            anno_unfilled_target_field_name = anno_unfilled_target[
                anno_unfilled_target.find("(") + 1: anno_unfilled_target.find(")")
            ]

            if type(spec) is Function:
                if param_index is None:
                    logout(f"ret type is '{spec.get_return_type()}'")
                    # Assumption: spec.return_type looks like 'struct <struct_name> ' or '<struct_name> '
                    # note: it'll look like '<struct_name> ' if a typedef is used

                    # we can make this assumption because:
                    # 1. param_index is none, which means we are making an
                    # annotation for the function itself, and (for now) we assume
                    # that means it's an annotation on its return.
                    # note: we'll need to update how we treat annotations
                    # applied to the function itself
                    # if/when annotations like @Owning and @MustCallAlias are implemented
                    # 2. the ".FIELD" in anno_unfilled_target is specified by the user via
                    # something like
                    '''
                        char *MustCall("free") 
                            creates_obligation(char *s Calls("free"), struct my_struct X Calls("free", "x"));
                                                                                                    ^^^
                        '''
                    # the calling of that second parameter on Calls/MustCall tells us that for the struct
                    # we are specifying, we are targeting some field. we verify whether that field
                    # actually exists, since this pass does
                    # keep note of every struct and its fields. if we wrote something like:
                    # void Calls("free", "x") example(int S Calls("free", "x")) { ... }
                    #                                 ^^^                 ^^^ implying "int" has field "x"
                    # we'll throw "ValueError: Did not find field 'x' for struct 'int', anno Calls target = _.FIELD(x) methods = free"

                    return_type_split = spec.get_return_type().split(" ")
                    if return_type_split[1] != "":
                        return_struct_name = return_type_split[1]
                    else:
                        return_struct_name = return_type_split[0]
                    struct = specifier_manager.get_struct(return_struct_name)

                    for field in struct.get_fields():
                        if anno_unfilled_target_field_name == field.get_field_name():
                            anno_unfilled_target_field_name = field.get_field_index()
                            break
                    else:
                        raise ValueError(
                            f"Did not find field '{anno_unfilled_target_field_name}' for struct '{struct.get_name()}', anno {anno_decl}")
                else:
                    # for the parameter, we need the type (the struct that it is definitely referring to)
                    found_field = False

                    spec_params = spec.get_parameters()

                    for param in spec_params:
                        if (found_field):
                            break

                        if param.get_index() == param_index:
                            # Assumption: param.param_type looks like 'struct <struct_name>' or '<struct_name>'
                            logout(
                                f"param_type_struct_name = '{param.get_param_type()}'")
                            try:
                                param_type_struct_name = param.get_param_type().split(" ")[
                                    1]
                            except IndexError:
                                param_type_struct_name = param.get_param_type()

                            struct = specifier_manager.get_struct(
                                param_type_struct_name)

                            for field in struct.get_fields():
                                if anno_unfilled_target_field_name == field.get_field_name():
                                    anno_unfilled_target_field_name = field.get_field_index()
                                    found_field = True
                                    break
                            else:
                                raise ValueError(
                                    f"Did not find field '{anno_unfilled_target_field_name}' for struct '{struct.get_name()}', anno {anno_decl}")

                    if (not found_field):
                        raise ValueError(
                            f"Did not find field '{anno_unfilled_target_field_name}' for anno '{anno_decl}', specifier '{spec.get_name()}'")

                logout(f"UNFILLED_FIELD = {anno_unfilled_target_field_name}")
                known_target += f".FIELD({anno_unfilled_target_field_name})"

        logout(
            f"for anno {anno_decl} known target is {known_target} and {spec.get_name()}")

        return AnnotateAttr(anno_type, known_target, anno_methods)

    def raw_ast_to_decl_type(self, ast_line: str, spec_manager: SpecifierManager) -> DeclType:
        expr = ast_line.strip()
        first_letter_index = expr.find(next(filter(str.isalpha, expr)))
        start_decl_index = first_letter_index
        decl_name = ""
        cur_decl_char = expr[start_decl_index]

        try:
            while (cur_decl_char != " "):
                decl_name += cur_decl_char
                start_decl_index += 1
                cur_decl_char = expr[start_decl_index]
        except IndexError:
            # might be a null statement (looks like "<<<NULL>>>" in AST)

            # TODO: write explicit check for null statements; lack of
            # documentation on the formatting of
            # null statements makes writing explicit checks difficult

            return None

        # reset param index and field index if needed
        if decl_name != "AnnotateAttr":
            if decl_name != "ParmVarDecl":
                self.__param_index = None
            if decl_name != "FieldDecl":
                self.__field_index = None

        # do parsing
        if decl_name == "FunctionDecl":
            return self._parse_function_decl(expr)

        elif decl_name == "RecordDecl":
            return self._parse_record_decl(expr)

        elif decl_name == "ParmVarDecl":
            self.__param_index = 0 if self.__param_index is None else (
                self.__param_index + 1)
            return self._parse_parm_var_decl(expr, self.__param_index)

        elif decl_name == "FieldDecl":
            self.__field_index = 0 if self.__field_index is None else (
                self.__field_index + 1)
            return self._parse_field_decl(expr, self.__field_index)

        elif decl_name == "TypedefDecl":
            return self._parse_typedef_decl(expr)

        elif decl_name == "VarDecl":
            return self._parse_var_decl(expr, spec_manager)

        elif decl_name == "AnnotateAttr":
            return self._parse_anno(expr, spec_manager, self.__param_index, self.__field_index)

        return None
