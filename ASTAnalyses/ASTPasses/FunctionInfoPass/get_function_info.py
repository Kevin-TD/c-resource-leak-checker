import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTPasses.get_ast_info_from_args import *

# (build dir)

# inserting ast info
# python3 ../ASTAnalyses/ASTPasses/FunctionInfoPass/get_function_info.py testASTOutput.txt annotationsOutput.txt

# inserting raw ast (ast info will be created)
# python3 ../ASTAnalyses/ASTPasses/FunctionInfoPass/get_function_info.py testAST.txt annotationsOutput.txt -raw

file_to_write_to = sys.argv[2]
ast_info = get_ast_info_from_args(sys.argv)

with open(file_to_write_to, "w") as output:
    output_str = ""

    for func in ast_info.get_functions():
        output_str += f"FUNCTION={func.get_name()}|RETURNS={func.get_return_type()}"

        if len(func.get_parameters()) != 0: 
            output_str += "|PARAMS|"

        for (i, param) in enumerate(func.get_parameters()):
            output_str += f"{param.get_index()}|{param.get_param_type()}"

            if (i != len(func.get_parameters()) - 1):
                output_str += "|"
                
        output_str += "\n"
    
    logout(output_str)
    output.write(f"{output_str}")