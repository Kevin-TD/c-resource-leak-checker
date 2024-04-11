# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/recover_desugar/main.c > testAST.txt ; python3 ../ASTInfoGenerator/ast_info_generator.py testAST.txt testASTOutput.txt

# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/test12/test12.c > testAST.txt ; python3 ../ASTInfoGenerator/ast_info_generator.py testAST.txt testASTOutput.txt

# python3 ../run_pt.py recover_desugar

import sys
sys.path.insert(0, '..')

from AnnoStructure.AnnotationManager import *
from Specifiers.FunctionStructure.Function import *
from Specifiers.StructStructure.Struct import *
from Specifiers.SpecifierManager import *
from StructVariables.StructVarManager import *
from ASTInfoGenerator.Debug import *

from DeclParser.DeclParser import *

file_to_read = sys.argv[1]
output_file = sys.argv[2]


def is_null_stmt(line_of_ast: str):
    # checks if end of string is <<<NULLL>>>

    null_section = line_of_ast.find("<<<NULL>>>")
    null_part_len = len("<<<NULL>>>")

    if null_section + null_part_len == len(line_of_ast):
        return True
    return False


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
    decl_parser = DeclParser()

    for expr in ast_lines:
        type_parsed = decl_parser.raw_ast_to_decl_type(expr, specifier_manager)
        found_type = type(type_parsed)

        if found_type is FunctionDecl:
            specifier_manager.add_function(
                type_parsed.get_fn_name(), type_parsed.get_ret_type())

        elif found_type is RecordDecl:
            specifier_manager.get_or_add_struct(type_parsed.get_struct_name())

        elif found_type is FieldDecl:
            spec = specifier_manager.get_specifiers()[-1]
            if type(spec) is Struct:
                field = Field(type_parsed.get_field_name(),
                              type_parsed.get_field_index())
                spec.add_field(field)

        elif found_type is ParmVarDecl:
            spec = specifier_manager.get_specifiers()[-1]
            if type(spec) is Function:
                param = Parameter(type_parsed.get_parm_index(),
                                  type_parsed.get_parm_type())
                spec.add_parameter(param)

        elif found_type is TypedefDecl:
            specifier_manager.get_or_add_struct(type_parsed.get_original_type_name()).add_typedef(
                type_parsed.get_alias_name())

        elif found_type is StructVarDecl:
            struct_var_manager.add_struct_var(
                type_parsed.get_struct_var_name(), type_parsed.get_struct_type_name())

        elif found_type is AnnotateAttr:
            anno = Annotation(type_parsed.get_annotation_type(
            ), type_parsed.get_annotation_target(), type_parsed.get_annotation_methods())
            annotation_manager.add_annotation(anno)

    output_str = ""

    # specifiers to AST Info language

    for spec in specifier_manager.get_specifiers():
        if type(spec) is Function:
            output_str += f"*FUNCTION\n"
            output_str += f"@NAME ({spec.get_name()})\n"
            output_str += f"@RETURN_TYPE ({spec.get_return_type().strip()})\n"

            # @PARAMETERS [type one,type two,type three]

            param_str = ""

            for (i, parameter) in enumerate(spec.get_parameters()):
                param_str += parameter.get_param_type()
                if (i != len(spec.get_parameters()) - 1):
                    param_str += ","

            if param_str != "":
                output_str += f"@PARAMETERS [{param_str.strip()}]\n"
            else: 
                output_str += "@PARAMETERS []\n"

            output_str += "\n"

    for spec in specifier_manager.get_specifiers():
        # *STRUCT
        # @NAME (struct_name)
        # @FIELDS [name1,name2,name3]

        if type(spec) is Struct:
            output_str += f"*STRUCT\n"
            output_str += f"@NAME ({spec.get_name()})\n"

            field_str = ""

            for (i, field) in enumerate(spec.get_fields()):
                field_str += field.get_field_name()
                if (i != len(spec.get_fields()) - 1):
                    field_str += ","

            if field_str != "":
                output_str += f"@FIELDS [{field_str.strip()}]\n"
            else:
                output_str += "@FIELDS []\n"

            output_str += "\n"

    for struct_var in struct_var_manager.struct_vars:
        output_str += "*STRUCT_VARIABLE\n"
        output_str += f"@NAME ({struct_var.get_var_name()})\n"
        output_str += f"@TYPE ({struct_var.get_type_name()})\n\n"

    for anno in annotation_manager.get_annotations():
        output_str += "*ANNOTATION\n"
        output_str += f"@ANNO_TYPE ({anno.get_anno_type()})\n"
        output_str += f"@TARGET ({anno.get_target()})\n"
        
        methods_str = anno.get_methods().split(",")
        methods_str = map(lambda x : x.strip(), methods_str)
        methods_str = ",".join(methods_str)

        output_str += f"@METHODS [{methods_str}]\n\n"
    

    logout(output_str)

    with open(output_file, "w") as output:
        output.write(output_str)
