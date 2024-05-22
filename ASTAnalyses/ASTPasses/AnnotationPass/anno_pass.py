import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTPasses.get_ast_info_from_args import *

# (build dir)

# inserting ast info
# python3 ../ASTAnalyses/ASTPasses/AnnotationPass/anno_pass.py testASTOutput.txt annotationsOutput.txt

# inserting raw ast (ast info will be created)
# python3 ../ASTAnalyses/ASTPasses/AnnotationPass/anno_pass.py testAST.txt annotationsOutput.txt -raw

# puts annotations into one file

file_to_write_to = sys.argv[2]
ast_info = get_ast_info_from_args(sys.argv)

with open(file_to_write_to, "w") as output:
    for (i, anno) in enumerate(ast_info.get_annotations()):
        output.write(anno.to_str())

        if (i != len(ast_info.get_annotations()) - 1):
            output.write("\n")
