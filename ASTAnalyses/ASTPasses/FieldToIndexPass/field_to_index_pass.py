import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTPasses.get_ast_info_from_args import *

# (build dir)

# inserting ast info
# python3 ../ASTAnalyses/ASTPasses/FieldToIndexPass/field_to_index_pass.py testASTOutput.txt annotationsOutput.txt

# inserting raw ast (ast info will be created)
# python3 ../ASTAnalyses/ASTPasses/FieldToIndexPass/field_to_index_pass.py testAST.txt annotationsOutput.txt -raw

# finds every variable whose type is a struct and creates a map of it and its field names to it and its field indices.
# example:
'''
struct my_struct {
 char* x;
 char* y;
}

int main() {
    struct my_struct A;
    struct my_struct B;
}
'''
# this C code will generate the following map:
'''
[A.x] => [A.0]
[A.y] => [A.1]
[B.x] => [B.0]
[B.y] => [B.1]
'''
# this is used for Testers/Passes when we want to specify a field of a variable by field name (rather than index)
# to test its MustCall/CalledMethods

file_to_write_to = sys.argv[2]
ast_info = get_ast_info_from_args(sys.argv)

with open(file_to_write_to, "w") as output:
    for (i, struct_var) in enumerate(ast_info.get_struct_vars()):
        logout(f"{struct_var.get_var_name()} {struct_var.get_type_name()}")

        for struct in ast_info.get_structs():
            if struct.get_name() == struct_var.get_type_name() or struct_var.get_type_name() in struct.get_typedefs():
                for found_field in struct.get_fields():
                    output_str = f"{struct_var.get_var_name()}.{found_field.get_field_name()}={struct_var.get_var_name()}.{found_field.get_field_index()}"
                    logout(f"writing {output_str}")
                    output.write(f"{output_str}\n")
