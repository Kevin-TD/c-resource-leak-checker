# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ./test/test23/test24.c > ./test/test23/test24_ast.txt

# TODO: if annotation is seen again, ensure they are the same 
# TODO: formalize code into classes; refraining from finalizing yet 
# just in case any unexpected cases pop up 
# TODO: write test cases for what generated annotations pop up 

# either RecordDecl or FunctionDecl
def parse_top(top: str):
    specifier = None # equals struct or function 
    name = ""  # refers to name of struct or function 

    # for RecordDecl
    # |-RecordDecl <pointer> <file ref> <line ref> struct <struct name> definition 

    # for FunctionDecl
    # |-FunctionDecl <pointer> <ref> col:<int> used <name> <ret type>
    # |-FunctionDecl <pointer> <ref> col:<int> <name> <ret type>
    # |-FunctionDecl <pointer> prev <pointer> <ref> line:<int>:<int>  used <name> <ret type>
    # |-FunctionDecl <pointer> prev <pointer> <ref> line:<int>:<int> <name> <ret type> 
    # ret type is always in quotes ''

    if top.startswith("|-RecordDecl"): 
        top_chunks = top.split(" ")
        specifier = "struct"
        name = top_chunks[len(top_chunks) - 2]

        
    elif top.startswith("|-FunctionDecl"):
        start_index = top.find("'") - 2
        cur_char = top[start_index]
        specifier = "function"

        while (cur_char != " "):
            name = cur_char + name

            start_index -= 1
            cur_char = top[start_index]
        
    return (specifier, name)

# either ParmVarDecl or FieldDecl
def parse_mid(mid: str, index):
    # could be on function itself (assuming it's on the return type for now), param, or field 
    target_type = "" # param, field, or self (function)
    target_name = "" # name of field or param ("" if function)

    if mid is None:
        target_type = "self"
        target_name = None

    elif "FieldDecl" in mid:
        target_type = "field"

        start_index = mid.find("'") - 2
        cur_char = mid[start_index]

        while (cur_char != " "):
            target_name = cur_char + target_name

            start_index -= 1
            cur_char = mid[start_index]
        
    
    elif "ParmVarDecl" in mid:
        target_type = "param"
        target_name = index - 1
    
    return (target_type, target_name)



def parse_anno(anno: str, top_res, mid_res):
    start_anno_index = anno.find('"')
    end_anno_index = anno.rfind('"')

    anno = anno[start_anno_index + 1 : end_anno_index]

    specifier, name = top_res
    target_type, target_name = mid_res

    full_target = ""

    print(anno, specifier, name, target_type, target_name, sep="\n", end="\n\n")

    if specifier == "struct":
        full_target = f" STRUCT({name}).FIELD({target_name})"
        
    elif specifier == "function":
        full_target = f" FUNCTION({name})"

        if target_type == "param":
            full_target += f".PARAM({target_name})"
            
    anno = anno.replace(" _", full_target)

    return anno 


        
    

with open("./test/test23/test24_ast.txt") as ast:
    cur_top_decl = ""  #FunctionDecl or RecordDecl
    cur_mid_decl = None #ParmVarDecl or FieldDecl
    param_index = None 
    # None means we are not at a FunctionDecl
    # 0 means we are at a FunctionalDecl 
    # 1, 2, ... means the (n-1)th parameter of that function (0 indexed)
    
    
    for expr in ast: 
        expr = expr.strip() 

        if "FunctionDecl" in expr:
            cur_top_decl = expr
            param_index = 0
            
        elif "RecordDecl" in expr:
            cur_top_decl = expr
            param_index = None 

        elif "ParmVarDecl" in expr: 
            cur_mid_decl = expr
            param_index += 1 

        elif "FieldDecl" in expr:
            cur_mid_decl = expr 
            param_index = None 
            
        elif "AnnotateAttr" in expr:
            annotation = expr

            if cur_mid_decl is None:
                param_index = None 

            # print(f"TOP: {cur_top_decl}\nMID: {cur_mid_decl}\nANNOTATION: {annotation}\nPARAM INDEX: {None if param_index is None else param_index - 1}\n")

            # print(f"TOP: {cur_top_decl}\n")
            # print(f"MID: {cur_mid_decl}\nPARAM INDEX: {None if param_index is None else param_index - 1}\n")
            # print(f"ANNOTATION: {annotation}")
            
            # parse_top(cur_top_decl)
            # parse_mid(cur_mid_decl, param_index)
            print(
                parse_anno(annotation, parse_top(cur_top_decl), parse_mid(cur_mid_decl, param_index))
            )


            cur_mid_decl = None