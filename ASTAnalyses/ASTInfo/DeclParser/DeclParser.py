from ASTAnalyses.ASTInfo.DeclParser.DeclType import DeclType
from ASTAnalyses.ASTInfo.Debug import *
from ASTAnalyses.ASTInfo.Specifiers.SpecifierManager import *

from ASTAnalyses.ASTInfo.Specifiers.StructStructure.Field import *
from ASTAnalyses.ASTInfo.Specifiers.FunctionStructure.Parameter import *

from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.FunctionDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.RecordDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.ParmVarDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.TypedefDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.FieldDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.StructVarDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.AnnotateAttr import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.BinaryOperatorDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.MemberExprDecl import *
from ASTAnalyses.ASTInfo.DeclParser.DeclTypes.DeclRefExprDecl import *


class DeclParser:
    def __init__(self):
        self.__param_index = None
        self.__field_index = None
        self.__prev_starting_decl_index = None

    def _parse_function_decl(self, function_decl: str) -> Specifier:
        logout(f"function decl equals {function_decl}")

        quote_index = function_decl.find("'")
        # Assumption: FunctionDecls wrap in quotes ('...') the return type of the function & parameters, and
        # right before the return type is the name of the function.
        # we also assume the function name and return type are separated by a space.
        # e.g., |-FunctionDecl 0x24161a8 <../test/test23/test23.h:27:1, line:28:57> col:5 does_something 'struct my_struct (struct my_struct)'
        #                                                                                 ^func name     ^return type      ^parameters

        # may also look like
        # |-FunctionDecl 0x24164a0 <../test/includes_test/call_structs.h:23:1, line:25:57> line:24:1 used creates_obligation 'char *(char *, struct my_struct)'

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
            # 3 parameters (in the order as it appears in C code), int, int, and char*

            return_type += cur_char

            start_index += 1
            cur_char = function_decl[start_index]

        # if return type looks like "int ", change it to "int"
        if return_type[len(return_type) - 1] == " ":
            return_type = return_type[:len(return_type) - 1]

        # if return type looks like "int *", change it to "int*"
        # or if it looks like "int **", change it to "int**"
        # (we assume asterisks are grouped together)
        potential_space_char_index = return_type.find("*") - 1
        if return_type[len(return_type) - 1] == "*" and return_type[potential_space_char_index] == " ":
            return_type = return_type[:potential_space_char_index] + \
                return_type[potential_space_char_index + 1:]

        logout(
            f"function name = '{function_name}', return type = '{return_type}'")
        return FunctionDecl(function_name, return_type)

    def _parse_record_decl(self, record_decl: str):
        record_decl_chunks = record_decl.split(" ")
        struct_name = record_decl_chunks[len(record_decl_chunks) - 2]
        logout(f"record decl {record_decl}")
        logout(f"struct name parsed = {struct_name}")

        return RecordDecl(struct_name)

    def _parse_param_var_decl(self, param_var_decl: str, param_index: int):
        # potential forms
        # ParmVarDecl 0x23e14d0 <col:12, col:18> col:18 used p 'void *'
        # ParmVarDecl 0x2412d70 <../test/test16/test16.c:21:66, col:83> col:83 used X 'struct my_struct':'struct my_struct'

        quote_second_index = param_var_decl.find(
            "'", param_var_decl.find("'") + 1)

        param_type = param_var_decl[
            param_var_decl.find("'") + 1: quote_second_index
        ]

        # check if param type is formatted like 'typedef_name':'struct_name *'
        # we extract it into just `struct_name`
        start_of_struct_type_name_index = quote_second_index + 3
        if len(param_var_decl) > start_of_struct_type_name_index and "*" in param_var_decl[start_of_struct_type_name_index:]:
            param_type = param_var_decl[start_of_struct_type_name_index: len(
                param_var_decl) - 1]
            param_type = param_type[: param_type.find("'")]

        logout(f"'{param_type}'")

        potential_space_char_index = param_type.find("*") - 1
        if param_type[len(param_type) - 1] == "*" and param_type[potential_space_char_index] == " ":
            param_type = param_type[:potential_space_char_index] + \
                param_type[potential_space_char_index + 1:]
        else:
            # param_type might look like "my_struct " or "struct my_struct"
            if param_type[-1] == " ":
                param_type = param_type[:len(param_type) - 1]

        logout(f"param added {param_type} for {param_var_decl}")

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

        # potential structures of a var decl
        # VarDecl 0x23e1d40 <col:5, col:11> col:11 used str 'char *'
        # VarDecl 0x23e1dd0 <col:5, col:21> col:9 a 'int' cinit
        # VarDecl 0x2414f60 <col:5, col:54> col:22 m_var_2 'struct my_struct':'struct my_struct' cinit
        # VarDecl 0x24141a0 <col:5, col:31> col:24 used K12 'struct my_struct2 *' cinit

        if var_decl_chunks[5] == "used":
            var_name = var_decl_chunks[6]
        elif var_decl_chunks[6] == "used":
            var_name = var_decl_chunks[7]
        else:
            var_name = var_decl_chunks[5]

        if "col" in var_name and ":" in var_name:
            var_name = var_decl_chunks[6]

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

        if struct_name == "struct" and len(struct_name_split) > 2:
            struct_name = struct_name_split[1]

        if not specifier_manager.struct_exists(struct_name):
            logout(
                f"did not find for parsed var_decl var name = {var_name} struct name = '{struct_name}'")
            return None

        logout(var_decl)
        logout(
            f"did parsed var_decl var name = {var_name} struct name = '{struct_name}'")

        return StructVarDecl(var_name, struct_name)

    def _parse_anno(self, anno_decl: str, specifier_manager: SpecifierManager, spec: Specifier, param_index: int, field_index: int):
        logout(f"pre is {anno_decl}")
        logout(f"parse anno spec is {spec.get_name()}")
        logout(f"param index = {param_index} ; field_index = {field_index}")

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
                    # annotation for the function itself, and we assume
                    # that means it's an annotation on its return.
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
                    if len(return_type_split) > 1 and return_type_split[1] != "":
                        return_struct_name = return_type_split[1]
                    else:
                        return_struct_name = return_type_split[0]
                    struct = specifier_manager.get_or_add_struct(
                        return_struct_name)

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

                            # removes "*" only for searching purposes
                            param_type_struct_name = param_type_struct_name.replace(
                                "*", "")

                            struct = specifier_manager.get_or_add_struct(
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

    def _parse_binary_operator_decl(self, binary_operator_decl: str):
        # may look like:
        # |-BinaryOperator 0x24128c0 <line:15:5, col:27> 'char *' '='
        # BinaryOperator 0x2412b58 <line:20:5, line:25:29> 'char *' '='
        # BinaryOperator 0x23e2c48 <col:21, col:25> 'int' '<'
        # we assume the binary operator is listed at the end

        decl_line_and_op_specification = binary_operator_decl[binary_operator_decl.find(
            "<"):]
        line_chunks = decl_line_and_op_specification.split(" ")

        if line_chunks[0].startswith("<col:"):
            return None

        # suitable if it looks like <line:15:5, col:27>
        line_number = int(line_chunks[0].split(":")[1])

        # check if it looks like <line:20:5, line:25:29>
        if line_chunks[1].startswith("line:"):
            line_number = int(line_chunks[1].split(":")[1])

        operator_type = line_chunks[len(line_chunks) - 1].replace("'", "")

        logout(f"op type '{operator_type}' line number '{line_number}'")

        if not BinaryOperatorDecl.is_valid_operator_type(operator_type):
            return None

        return BinaryOperatorDecl(operator_type, line_number)

    def _parse_member_expr_decl(self, member_expr_decl: str):
        # may look like:
        # |   `-MemberExpr 0x24129f8 <col:10, col:12> 'char *' lvalue .x 0x2412340

        l_value_section = member_expr_decl[member_expr_decl.find("lvalue"):]
        line_chunks = l_value_section.split(" ")

        field_specified = line_chunks[1]

        logout(f"lvalue field specified '{field_specified}'")

        return MemberExprDecl(field_specified)

    def _parse_decl_ref_expr(self, decl_ref_expr_decl: str):
        # may look like:
        # `-DeclRefExpr 0x2412930 <col:11> 'struct M':'struct M' lvalue Var 0x2412650 's' 'struct M':'struct M'
        # |-DeclRefExpr 0x24129b8 <col:5> 'char *' lvalue Var 0x24126e0 's1' 'char *'
        # `-DeclRefExpr 0x2386cc8 <col:10> '__uint16_t':'unsigned short' lvalue ParmVar 0x2386b40 '__x' '__uint16_t':'unsigned short'

        if decl_ref_expr_decl.find("lvalue") == -1:
            return None

        var_section = decl_ref_expr_decl[decl_ref_expr_decl.find("lvalue"):]
        line_chunks = var_section.split(" ")

        var_name = line_chunks[3].replace("'", "")

        logout(f"decl ref expr var name = '{var_name}'")

        return DeclRefExprDecl(var_name)

    def _is_null_stmt(self, line_of_ast: str):
        # checks if end of string is <<<NULLL>>>

        null_section = line_of_ast.find("<<<NULL>>>")
        null_part_len = len("<<<NULL>>>")

        if null_section + null_part_len == len(line_of_ast):
            return True
        return False

    def raw_ast_to_decl_type(self, ast_line: str, spec_manager: SpecifierManager, recent_spec: Specifier) -> DeclType:
        expr = ast_line.strip()
        start_decl_index = expr.find(next(filter(str.isalpha, expr)))
        decl_name = ""
        cur_decl_char = expr[start_decl_index]

        if self._is_null_stmt(expr):
            self.__param_index = None
            self.__field_index = None
            return None

        while (cur_decl_char != " "):
            decl_name += cur_decl_char
            start_decl_index += 1
            cur_decl_char = expr[start_decl_index]

        # reset param index and field index if needed
        if decl_name != "AnnotateAttr":
            if decl_name != "ParmVarDecl":
                self.__param_index = None
            if decl_name != "FieldDecl":
                self.__field_index = None
        else:
            # check in place for the following case

            # (1) |-FunctionDecl 0x24164a0 <../test/includes_test/call_structs.h:23:1, line:25:57> line:24:1 used creates_obligation 'char *(char *, struct my_struct)'
            # (2) | |-ParmVarDecl 0x2416170 <col:20, col:26> col:26 s 'char *'
            # (3) | | `-AnnotateAttr 0x24161d0 <../test/includes_test/../../Annotations/Annotations.h:2:18, line:3:39> "Calls target = _ methods = free"
            # (4) | |-ParmVarDecl 0x2416320 <../test/includes_test/call_structs.h:25:20, col:37> col:37 X 'struct my_struct':'struct my_struct'
            # (5) | | `-AnnotateAttr 0x2416380 <../test/includes_test/../../Annotations/Annotations.h:6:8, col:71> "Calls target = _.FIELD(x) methods = free"
            # (6) | `-AnnotateAttr 0x2416548 <line:14:18, col:67> "MustCall target = _ methods = free"

            # at (6) we need to know that the AnnotateAttr is referring to the function's return,
            # not the ParmVarDecl. we do this by checking if the starting index has shifted left.

            if start_decl_index < self.__starting_decl_index:
                self.__param_index = None
                self.__field_index = None

        self.__starting_decl_index = start_decl_index

        # do parsing
        if decl_name == "FunctionDecl":
            return self._parse_function_decl(expr)

        elif decl_name == "RecordDecl":
            return self._parse_record_decl(expr)

        elif decl_name == "ParmVarDecl":
            self.__param_index = 0 if self.__param_index is None else (
                self.__param_index + 1)
            return self._parse_param_var_decl(expr, self.__param_index)

        elif decl_name == "FieldDecl":
            self.__field_index = 0 if self.__field_index is None else (
                self.__field_index + 1)
            return self._parse_field_decl(expr, self.__field_index)

        elif decl_name == "TypedefDecl":
            return self._parse_typedef_decl(expr)

        elif decl_name == "VarDecl":
            return self._parse_var_decl(expr, spec_manager)

        elif decl_name == "AnnotateAttr":
            return self._parse_anno(expr, spec_manager, recent_spec, self.__param_index, self.__field_index)

        elif decl_name == "BinaryOperator":
            return self._parse_binary_operator_decl(expr)

        elif decl_name == "MemberExpr":
            return self._parse_member_expr_decl(expr)

        elif decl_name == "DeclRefExpr":
            return self._parse_decl_ref_expr(expr)

        return None
