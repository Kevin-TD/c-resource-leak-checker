# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/recover_desugar/main.c > testAST.txt ; python3 ../ASTInfoGenerator/ast_info_generator.py testAST.txt testASTOutput.txt

# python3 ../run_pt.py recover_desugar


# random_data
# my_struct
# |-RecordDecl 0x2413cf8 <../test/recover_desugar/main.c:6:9, line:9:1> line:6:16 struct my_struct definition

import sys
sys.path.insert(0, '..')

from AnnoStructure.AnnotationManager import *
from Specifiers.FunctionStructure.Function import *
from Specifiers.StructStructure.Struct import *
from Specifiers.SpecifierManager import *
from StructVariables.StructVarManager import *
from ASTInfoGenerator.Debug import *


def parse_function_decl(function_decl: str, specifier_manger: SpecifierManager) -> Specifier:
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

    logout(f"function name = '{function_name}', return type = '{return_type}'")
    return specifier_manger.add_function(function_name, return_type)


def parse_record_decl(record_decl: str, specifier_manger: SpecifierManager):
    record_decl_chunks = record_decl.split(" ")
    struct_name = record_decl_chunks[len(record_decl_chunks) - 2]

    return specifier_manger.add_struct(struct_name)


def parse_parm_var_decl(parm_var_decl: str, param_index: int, func: Specifier):
    if type(func) != Function:
        return

    logout(f"adding param {parm_var_decl}")
    quote_second_index = find_second_index(parm_var_decl, "'")

    param_type = parm_var_decl[
        parm_var_decl.find("'") + 1: quote_second_index
    ]

    # checks pointer type is specified

    # check if param type is formatted like 'struct_name *'
    # we extract it into just `struct_name`
    if "*" in param_type:
        param_type = param_type[: param_type.find(" ")]

    # check if param type is formatted like 'typedef_name':'struct_name *'
    # we extract it into just `struct_name`
    start_of_struct_type_name_index = quote_second_index + 3
    if len(parm_var_decl) > start_of_struct_type_name_index and "*" in parm_var_decl[start_of_struct_type_name_index:]:
        param_type = parm_var_decl[start_of_struct_type_name_index: len(
            parm_var_decl) - 1]
        param_type = param_type[: param_type.find(" ")]

    logout(f"param added {param_type}")

    param = Parameter(param_index, param_type)
    func.add_parameter(param)


def parse_field_decl(field_decl: str, field_index: int, struct: Specifier):
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


# finds the index of the 2nd occurrence of char in string
def find_second_index(string: str, char: str) -> int:
    return string.find(char, string.find(char) + 1)

def parse_typedef_decl(specifier_manager_holder: SpecifierManager, typedef_decl: str):
    if not ("referenced" in typedef_decl and "struct" in typedef_decl):
        return

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
        specifier_manager_holder.get_struct(
            original_name).add_typedef(typedef_alias_name)
        logout(f"added typedef '{original_name}' '{typedef_alias_name}'")

def parse_var_decl(var_decl: str, struct_var_manager_holder: StructVarManager, specifier_manager_holder: SpecifierManager):

    var_decl_chunks = var_decl.split(" ")

    logout(var_decl)
    logout(var_decl_chunks)

    if var_decl_chunks[5] == "used":
        var_name = var_decl_chunks[6]
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

    if not specifier_manager_holder.struct_exists(struct_name):
        logout(
            f"did not find for parsed var_decl var name = {var_name} struct name = '{struct_name}'")
        return

    logout(var_decl)
    logout(
        f"parsed var_decl var name = {var_name} struct name = '{struct_name}'")

    struct_var_manager_holder.add_struct_var(var_name, struct_name)

def parse_anno(anno: str, spec: Specifier, annotation_manager: AnnotationManager, param_index: int = None, field_index: int = None):
    logout(f"pre is {anno}")

    start_anno_index = anno.find('"')
    end_anno_index = anno.rfind('"')

    anno = anno[start_anno_index + 1: end_anno_index]

    anno_type = anno[
        0: anno.find(" ")
    ]

    anno_methods = anno[anno.rfind("methods = ") + len("methods = ")::]
    logout(f"ANNO METHODS = {anno_methods}")

    known_target = ""

    anno_unfilled_target = ""
    anno_unfilled_target_index = anno.find("_")

    while (anno[anno_unfilled_target_index] != " "):
        anno_unfilled_target += anno[anno_unfilled_target_index]
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
                        f"Did not find field '{anno_unfilled_target_field_name}' for struct '{struct.get_name()}', anno {anno}")
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
                                f"Did not find field '{anno_unfilled_target_field_name}' for struct '{struct.get_name()}', anno {anno}")

                if (not found_field):
                    raise ValueError(
                        f"Did not find field '{anno_unfilled_target_field_name}' for anno '{anno}', specifier '{spec.get_name()}'")

            logout(f"UNFILLED_FIELD = {anno_unfilled_target_field_name}")
            known_target += f".FIELD({anno_unfilled_target_field_name})"

    logout(
        f"for anno {anno} known target is {known_target} and {spec.get_name()}")
    built_anno = Annotation(anno_type, known_target, anno_methods)
    annotation_manager.add_annotation(built_anno)


file_to_read = sys.argv[1]
output_file = sys.argv[2]


with open(file_to_read) as ast:
    cur_spec = None  # Function or Struct or None
    cur_top_decl = None  # FunctionDecl or RecordDecl (strings)
    cur_mid_decl = None  # ParmVarDecl or FieldDecl (strings)
    param_index = None
    field_index = None

    ast_lines = ast.readlines()
    specifier_manager = SpecifierManager()
    annotation_manager = AnnotationManager()
    struct_var_manager = StructVarManager()

    for expr in ast_lines:
        expr = expr.strip()

        first_letter_index = expr.find(next(filter(str.isalpha, expr)))
        start_decl_index = first_letter_index
        decl = ""
        cur_decl_char = expr[start_decl_index]

        try:
            while (cur_decl_char != " "):
                decl += cur_decl_char
                start_decl_index += 1
                cur_decl_char = expr[start_decl_index]
        except IndexError:
            # might be a null statement (looks like "<<<NULL>>>" in AST)

            # TODO: write explicit check for null statements; lack of
            # documentation on the formatting of
            # null statements makes writing explicit checks difficult

            field_index = None
            continue

        if "FunctionDecl" == decl:
            cur_top_decl = expr
            param_index = None
            field_index = None
            logout(f"function decl expr = {cur_top_decl}")
            cur_spec = parse_function_decl(cur_top_decl, specifier_manager)

        elif "RecordDecl" == decl:
            cur_top_decl = expr
            param_index = None
            field_index = None
            logout(f"record decl expr = {cur_top_decl}")
            cur_spec = parse_record_decl(cur_top_decl, specifier_manager)

        elif "ParmVarDecl" == decl:
            cur_mid_decl = expr
            field_index = None

            if param_index is None:
                param_index = 0
            else:
                param_index += 1
            logout(f"parm var decl = {cur_mid_decl}")
            parse_parm_var_decl(cur_mid_decl, param_index, cur_spec)
        
        elif "TypedefDecl" == decl:
            cur_top_decl = expr
            field_index = None
            logout(f"type def decl = {cur_top_decl}")
            parse_typedef_decl(specifier_manager, cur_top_decl)

        elif "FieldDecl" == decl:
            cur_mid_decl = expr
            param_index = None

            if field_index is None:
                field_index = 0
            else:
                field_index += 1

            logout(f"field decl = {cur_mid_decl}")
            parse_field_decl(cur_mid_decl, field_index, cur_spec)

        elif "AnnotateAttr" == decl:
            annotation = expr

            if cur_mid_decl is None:
                param_index = None
            
            logout(f"annotate attr = {expr}")

            logout(
                f"top: {cur_top_decl}\nmid: {cur_mid_decl}\nanno: {annotation}\nparam_index: {param_index}\nfield_index: {field_index}\n")
            parse_anno(annotation, cur_spec, annotation_manager,
                       param_index, field_index)

            # if there's a "|-" on an annotation expression, we assume there is
            # at least 1 more annotation,
            # so "context" (cur_mid_decl) is not removed for the next line
            if "|-" not in expr:
                cur_mid_decl = None

        else:
            if "VarDecl" == decl and "struct" in expr:
                logout(f"var decl struct = {expr[first_letter_index:]}")

                parse_var_decl(expr[first_letter_index:],
                               struct_var_manager, specifier_manager)
                
            param_index = None
            field_index = None

    if DEBUG:
        for spec in specifier_manager.specifiers:
            if type(spec) is Function:
                print(f"*FUNCTION")
                print(f"@NAME ({spec.get_name()})")
                print(f"@RETURN_TYPE ({spec.get_return_type().strip()})")

                # @PARAMETERS [type one,type two,type three]

                param_str = ""

                for (i, parameter) in enumerate(spec.get_parameters()):
                    param_str += parameter.get_param_type()
                    if (i != len(spec.get_parameters()) - 1):
                        param_str += ","
                
                if param_str != "":
                    print(f"@PARAMETERS [{param_str.strip()}]")
                
                print()

                    
        
        for spec in specifier_manager.specifiers:
            # *STRUCT 
            # @NAME (struct_name)
            # @FIELDS [name1,name2,name3]
    
            if type(spec) is Struct:
                print(f"*STRUCT")
                print(f"@NAME ({spec.get_name()})")

                field_str = ""

                for (i, field) in enumerate(spec.get_fields()):
                    field_str += field.get_field_name()
                    if (i != len(spec.get_fields()) - 1):
                        field_str += ","
                
                if field_str != "":
                    print(f"@FIELDS [{field_str.strip()}]")
                
                print()
        
        for struct_var in struct_var_manager.struct_vars:
            print("*STRUCT_VARIABLE_DECLARATION")
            print(f"@NAME ({struct_var.get_var_name()})")
            print(f"@TYPE ({struct_var.get_type_name()})")
            print()
        
        for anno in annotation_manager.get_annotations():
            print("*ANNOTATION")
            print(f"@STRING ({anno.to_str()})")
            print()
    
    '''
    *FUNCTION 
    @NAME (function_name)
    @RETURN_TYPE (return type)
    @PARAMETERS [type one,type two,type three]
    
    *STRUCT 
    @NAME (struct_name)
    @FIELDS [name1,name2,name3]

    *STRUCT_VARIABLE_DECLARATION
    @NAME (var_name)
    @TYPE (structty)

    *ANNOTATION
    @STRING (MustCall target = STRUCT(my_struct).FIELD(0) methods = free))

    '''

    with open(output_file, "w") as output:
        pass
        # for i in range(len(annotation_manager.annotations)):
        #     anno = annotation_manager.annotations[i]

        #     output.write(anno.to_str())
        #     if LOG_ANNOTATIONS:
        #         print(anno.to_str())

        #     if i != len(annotation_manager.annotations) - 1:
        #         output.write("\n")
