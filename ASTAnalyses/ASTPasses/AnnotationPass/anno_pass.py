import sys
import tempfile
import os
sys.path.insert(0, '..')

from ASTAnalyses.ASTInfo.reader import *
from ASTAnalyses.ASTInfo.generator import *

# (build dir)

# inserting ast info
# python3 ../ASTAnalyses/ASTPasses/AnnotationPass/anno_pass.py testASTOutput.txt annotationsOutput.txt

# inserting raw ast (ast info will be created)
# python3 ../ASTAnalyses/ASTPasses/AnnotationPass/anno_pass.py testASTOutput.txt annotationsOutput.txt -raw

# puts annotations into one file
# takes in AST

file_to_read = sys.argv[1]
file_to_write_to = sys.argv[2]
raw_given = False

if len(sys.argv) > 3:
    if sys.argv[3] == "-raw":
        raw_given = True
    else:
        raise SyntaxError(
            f"Unknown flag {sys.argv[3]}. Expected -raw or no flag specified")


ast_info = None

if raw_given:
    with tempfile.NamedTemporaryFile() as fp:
        command = f"python3 ../ASTAnalyses/ASTInfo/generator.py {file_to_read} {fp.name}"
        run_generate_status = os.system(command)
        if run_generate_status != 0:
            raise RuntimeError("Error generating AST info from raw AST")
        ast_info = ASTReader(fp.name)
else:
    ast_info = ASTReader(file_to_read)

with open(file_to_write_to, "w") as output:
    for (i, anno) in enumerate(ast_info.get_annotations()):
        output.write(anno.to_str())

        if (i != len(ast_info.get_annotations()) - 1):
            output.write("\n")
