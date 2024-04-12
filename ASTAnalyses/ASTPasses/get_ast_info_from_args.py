import sys
import tempfile
import os
sys.path.insert(0, '..')

from ASTAnalyses.ASTInfo.reader import *

# inserting ast info
# python3 [pass name] [ast info].txt annotationsOutput.txt

# inserting raw ast (ast info will be created)
# python3 [pass name] [raw ast].txt annotationsOutput.txt -raw


def get_ast_info_from_args(args):
    """
    translates sys.argv arguments into AST info, whether the parsed AST info has been passed in 
    or the raw AST has been passed 
    """

    file_to_read = args[1]
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

    return ast_info
