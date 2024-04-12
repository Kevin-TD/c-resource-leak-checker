import sys
sys.path.insert(0, '..')

from ASTAnalyses.ASTInfo.reader import *

# (build dir)
# python3 ../ASTAnalyses/ASTPasses/AnnotationPass/anno_pass.py testASTOutput.txt annotationsOutput.txt

file_to_read = sys.argv[1]
file_to_write_to = sys.argv[2]
ast_info = ASTReader(file_to_read)

with open(file_to_write_to, "w") as output:
    for (i, anno) in enumerate(ast_info.get_annotations()):
        output.write(anno.to_str())

        if (i != len(ast_info.get_annotations()) - 1):
            output.write("\n")
