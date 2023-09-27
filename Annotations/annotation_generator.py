#! python3 annotation_generator.py <input file name (AST)> <output file name (Annotations)>

from enum import Enum 
import sys 

# TODO: if annotation is seen again, ensure they are the same 
# TODO: formalize code into classes; refraining from finalizing yet 
# just in case any unexpected cases pop up 
# TODO: write test cases for what generated annotations pop up 

# parse methods take advantage of the formatting of the AST dumper,
# and are thus, very error prone 

# considering making this a C++ file instead since it's getting a little long 

# TOOL_CHECKER Calls target = FUNCTION(does_something).PARAM(0).FIELD(x) methods = free
#                                                               ^^^^ this is a problem

#! NEED TO GRAB RETURN TYPE. AVAILABLE ON FunctionDecl IN QUOTES '

# formatting:
# 'char *(char *, struct my_struct)'
#  ^ ret   ^param1  ^param2    (types)

# 'void ()'  for no params 
# go from first char, keep collecting until ( is reached

# does_something 'struct my_struct (struct my_struct)'
# just grabbing the first "struct my_struct ". split on spaces and grab split[1] (the 2nd)



class Annotation:
    def __init__(self, anno_type, target, methods):
        self.anno_type = anno_type
        self.target = target 
        self.methods = methods 
    
    def __eq__(self, otherAnno):
        return self.anno_type == otherAnno.anno_type and self.target == otherAnno.target  
        
    def methodsAreEqualWith(self, otherAnno):
        return self.methods == otherAnno.methods 

    def getAnnotationString(self):
        return f"TOOL_CHECKER {self.anno_type} target = {self.target} methods = {self.methods}"

class Struct:
    def __init__(self, struct_name):
        self.struct_name = struct_name
        self.fields = []

class StructsManager:
    def __init__(self):
        self.structs = [] 
    
    def add_struct(self, struct_name):
        self.structs.append(Struct(struct_name))
    
    def get_struct(self, struct_name):
        for struct in self.structs:
            if struct.struct_name == struct_name:
                return struct
        
        new_struct = Struct(struct_name)
        self.structs.append(new_struct)
        return self.structs[-1]
    
    def add(self, struct_name, field_name):
        struct = self.get_struct(struct_name)
        struct.fields.append(field_name)

class Specifier(Enum):
    Init = 0
    Struct = 1
    Function = 2

class Target(Enum):
    Init = 0
    Param = 1
    Field = 2 
    FunctionSelf = 3 



# either RecordDecl or FunctionDecl
def parse_top(top):
    specifier = Specifier.Init # equals struct or function 
    specifier_name = ""  

    if "RecordDecl" in top:
        top_chunks = top.split(" ")
        specifier = Specifier.Struct
        specifier_name = top_chunks[len(top_chunks) - 2]

        
    elif "FunctionDecl" in top:
        start_index = top.find("'") - 2
        cur_char = top[start_index]
        specifier = Specifier.Function

        while (cur_char != " "):
            specifier_name = cur_char + specifier_name

            start_index -= 1
            cur_char = top[start_index]
        
    return (specifier, specifier_name)

# either ParmVarDecl or FieldDecl
def parse_mid(mid, param_index, field_index):
    target_type = Target.Init 
    target_name = "" 

    if mid is None:
        target_type = Target.FunctionSelf
        target_name = None

    elif "FieldDecl" in mid:
        target_type = Target.Field
        # target_name = str(field_index)

        start_index = mid.find("'") - 2
        cur_char = mid[start_index]

        while (cur_char != " "):
            target_name = cur_char + target_name

            start_index -= 1
            cur_char = mid[start_index]
        
        target_name += " " + str(field_index)
        
    
    elif "ParmVarDecl" in mid:
        target_type = Target.Param
        target_name = param_index - 1
    
    return (target_type, target_name)

def add_if_struct(top_res, mid_res, struct_manager):
    specifier, specifier_name = top_res
    target_type, target_name = mid_res 

    if specifier is Specifier.Struct:
        struct_manager.add(specifier_name, target_name)




def parse_anno(anno, all_annotations, structs_manager, top_res, mid_res):
    start_anno_index = anno.find('"')
    end_anno_index = anno.rfind('"')

    anno = anno[start_anno_index + 1 : end_anno_index]

    specifier, name = top_res
    target_type, target_name = mid_res

    full_target = ""

    if specifier is Specifier.Struct: 
        target_index = target_name.split(" ")[1]
        full_target = f"STRUCT({name}).FIELD({target_index})"
        
    elif specifier is Specifier.Function: 
        full_target = f"FUNCTION({name})"

        if target_type is Target.Param: 
            full_target += f".PARAM({target_name})"
        
        # assuming that if the target is self, it's referencing the function's return 
        elif target_type is Target.FunctionSelf:
            full_target += ".RETURN"


    # finds the index of the 2nd occurrence of char in string 
    find_second_index = lambda string, char : string.find(char, string.find(char) + 1)

    anno_type = anno[
        anno.find(" ") + 1 : find_second_index(anno, " ")
    ]

    methods = anno[anno.rfind(" ") + 1::]

    anno_unfilled_target = ""
    anno_unfilled_target_index = find_second_index(anno, "_")

    while (anno[anno_unfilled_target_index] != " "):
        anno_unfilled_target += anno[anno_unfilled_target_index]
        anno_unfilled_target_index += 1
        

    full_target = anno_unfilled_target.replace("_", full_target)

    if ".FIELD" in anno_unfilled_target:
        # this is a FUNCTION
        print(f"unfilled target contains .field {anno_unfilled_target} | {anno} | {target_name} | {specifier} | {name} | {full_target}")

        
        target_split = target_name.split(" ")
        struct_name, struct_index = target_split[0], target_split[1]

        struct_fields = structs_manager.get_struct(struct_name).fields
        print(f"fields = {struct_fields}")

    full_anno = Annotation(anno_type, full_target, methods)

    for annotation in all_annotations:
        if annotation == full_anno:
            if not annotation.methodsAreEqualWith(full_anno):
                raise ValueError(
                    f"Two annotations with same type ('{annotation.anno_type}') and target ('{annotation.target}') with differing methods ('{annotation.methods}' and '{full_anno.methods}')"
                )
    
    if full_anno not in all_annotations:
        all_annotations.append(full_anno)

    return anno 
    
fileToRead = sys.argv[1]
outputFile = sys.argv[2]


with open(fileToRead) as ast:
    cur_top_decl = None  #FunctionDecl or RecordDecl
    cur_mid_decl = None #ParmVarDecl or FieldDecl
    param_index = None 
    field_index = None
    all_annotations = [] 
    all_structs = StructsManager()

    ast_lines = ast.readlines()
    
    
    for (line, expr) in enumerate(ast_lines): 
        expr = expr.strip() 

        if "FunctionDecl" in expr:
            cur_top_decl = expr
            param_index = 0
            field_index = None 
            
        elif "RecordDecl" in expr:
            cur_top_decl = expr
            param_index = None 
            field_index = None 

        elif "ParmVarDecl" in expr: 
            cur_mid_decl = expr
            param_index += 1 
            field_index = None 

        elif "FieldDecl" in expr:
            cur_mid_decl = expr 
            param_index = None 
            
            if field_index is None: 
                field_index = 0
            else:
                field_index += 1
            
            add_if_struct(parse_top(cur_top_decl), parse_mid(cur_mid_decl, param_index, field_index), all_structs)
            
        elif "AnnotateAttr" in expr:
            annotation = expr

            if cur_mid_decl is None:
                param_index = None  

            
            parse_anno(annotation, all_annotations, all_structs, parse_top(cur_top_decl), parse_mid(cur_mid_decl, param_index, field_index))   
            
            # if there's a "|-" on an annotation expression, we assume there are 2 precisely 
            # annotations on that target, 
            # (probably one in the .c and one in the .h). 
            # so "context" (cur_mid_decl) is not removed for the next line 
            if "|-" not in expr: 
                cur_mid_decl = None

    with open(outputFile, "w") as testFile:
        for i in range(len(all_annotations)):
            anno = all_annotations[i]
            
            testFile.write(anno.getAnnotationString())

            if i != len(all_annotations) - 1:
                testFile.write("\n")