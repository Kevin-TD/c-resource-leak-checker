import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTPasses.get_ast_info_from_args import *

# inserting ast info
# python3 ../ASTAnalyses/ASTPasses/LValuePass/l_value_pass.py testASTOutput.txt annotationsOutput.txt

# inserting raw ast (ast info will be created)
# python3 ../ASTAnalyses/ASTPasses/LValuePass/l_value_pass.py testAST.txt annotationsOutput.txt -raw

# maps line numbers to l-value names
# example program:
'''
(1) a = 1 
(2) b = 2 
(3) c 
(4)    = 
(5) 3 

our output is then 
1=a
2=b
5=c

with the LHS referring to the line number of the l-value 
and the RHS referring to the l-value 

note: 5=c because it matches what the LLVM IR does in terms
of listing the line number for each instruction
'''

file_to_write_to = sys.argv[2]
ast_info = get_ast_info_from_args(sys.argv)

with open(file_to_write_to, "w") as output:
    for (i, l_value) in enumerate(ast_info.get_l_values()):
        output.write(
            str(l_value.get_line_number()) + "=" + l_value.get_lhs_name()
        )

        if (i != len(ast_info.get_l_values()) - 1):
            output.write("\n")
