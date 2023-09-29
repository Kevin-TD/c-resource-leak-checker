import sys 
from abc import ABC as AbstractClass

# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/test20.c > CodeAnalyzerFiles/test20_AST.txt ; python3 ../Annotations/annoGen.py CodeAnalyzerFiles/test20_AST.txt CodeAnalyzerFiles/test20_ANNOTATIONS.txt

# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/test23/test24.c > CodeAnalyzerFiles/test24_AST.txt ; python3 ../Annotations/annoGen.py CodeAnalyzerFiles/test24_AST.txt CodeAnalyzerFiles/test24_ANNOTATIONS.txt

class Annotation:
    def __init__(self, anno_type: str, target: str, methods: str):
        self.anno_type = anno_type
        self.target = target 
        self.methods = methods 
    
    # other: Annotation
    def __eq__(self, other) -> bool:
        return self.anno_type == other.anno_type and self.target == other.target  
    
    # other: Annotation
    def methods_equal(self, other) -> bool:
        return self.methods == other.methods 

    def to_str(self) -> str:
        return f"TOOL_CHECKER {self.anno_type} target = {self.target} methods = {self.methods}"

class AnnotationManager:
    def __init__(self):
        self.annotations: list[Annotation] = []
    
    def add_annotation(self, anno: Annotation):
        for annotation in self.annotations:
            if annotation == anno:
                if not annotation.methods_equal(anno):
                    raise ValueError(
                        f"Two annotations with same type ('{annotation.anno_type}') and target ('{annotation.target}') with differing methods ('{annotation.methods}' and '{anno.methods}')"
                    )
        
        if anno not in self.annotations:
            self.annotations.append(anno)
    

# note: annotations are not linked to a field or parameter because
# it can be considerably more effort to find what field/parameter/struct/field 
# an annotation belongs to; we are only doing string comparisons between
# annotations (to ensure that if two annotations of the same
# type and target ALSO have the same methods) so there is not much to lose out on 

class Field:
    def __init__(self, field_name: str, field_index: int):
        self.field_name = field_name
        self.field_index = field_index

class Parameter:
    def __init__(self, index: int, param_type: str):
        self.index = index 
        self.param_type = param_type


class Specifier(AbstractClass):
    def __init__(self, name: str):
        self.name = name
    
    def get_name(self) -> str: 
        return self.name

class Function(Specifier):
    def __init__(self, name: str, return_type: str):
        self.name = name 
        self.return_type = return_type 
        self.parameters: list[Parameter] = []
    
    def set_return_type(self, return_type: str) -> None: 
        self.return_type = return_type
    
    def add_parameter(self, parameter: Parameter) -> None: 
        self.parameters.append(parameter)

class Struct(Specifier):
    def __init__(self, name: str):
        self.name = name
        self.fields: list[Field] = []
    
    def add_field(self, field: Field) -> None:
        self.fields.append(field)

class SpecifierManager:
    def __init__(self):
        self.specifiers: list[Specifier] = []
    
    # if function is not found, a new one is returned with name function_name 
    def get_function(self, function_name: str) -> Function:
        for specifier in self.specifiers:
            if specifier.get_name() == function_name:
                return specifier
        
        new_function = Function(function_name, None)
        self.specifiers.append(new_function)
        return self.specifiers[-1]
    
    # if struct is not found, a new one is returned with name struct_name
    def get_struct(self, struct_name: str) -> Struct:
        for specifier in self.specifiers:
            if specifier.get_name() == struct_name:
                return specifier
        
        new_struct = Struct(struct_name)
        self.specifiers.append(new_struct)
        return self.specifiers[-1] 
    
    # if there already exists a function of name function_name, nothing is added (false returned).
    # otherwise, the function is added (true returned)
    def add_function(self, function_name: str, return_type: str) -> Specifier:
        for specifier in self.specifiers:
            if specifier.get_name() == function_name: 
                # we append and remove to shift specifier to the last item 
                # of self.specifiers. this is required since we   
                # assume that the last index of self.specifiers is 
                # referring to some function/struct/annotation we are building on.

                # what's meant by "building on" is that FunctionDecls/RecordDecls, 
                # ParmVarDecls/FieldDecls, and AnnotateAttrs are bundled together 
                # in the AST. so, once we see a FunctionDecl or RecordDecl, 
                # we assume all following ParmVarsDecls/RecordDecls and AnnotateAttrs
                # belong to that Function/RecordDecl. Once we encounter another
                # Function/RecordDecl, we know we have finished looking at the previous one. 

                # here's an example of how it looks like impl wise: 
                # we have our list of specifiers S = []. 
                # we come across a new FunctionDecl f, and add it to S. so 
                # S = [f]. we then come across a ParmVarDecl p, and we assume
                # it belongs to f as we also assume we have not entered a different FunctionDecl 
                # (note that nested functions cannot exist in ANSI C, but in Gnu C; Gnu C is much less standard anyhow). 
                # accessing f will simply be at S[-1] (last index), so we can do S[-1].addParam(p). 
                # then we come across another FunctionDecl f1, so S = [f, f1]. we come across
                # another param p1 and this'll be referring to S[-1] again, so we do the same S[-1].addParam(p1).

                # now, what can happen in the AST is the same function can appear several times. 
                # we still want to analyze the function in case there is new annotation info, 
                # but we do not want to add it as a new function. that information 
                # we got from that function appearing previous could be useful, such as 
                # detecting if some annotation from the .c and .h files differ when they shouldn't. 
                # thus, we shift the specifier to the end so that code is working 
                # with the proper specifier 

                # self.specifiers.remove(specifier)
                # self.specifiers.append(specifier)
                return specifier 
            
        new_specifier = Function(function_name, return_type)
        self.specifiers.append(new_specifier)
        return self.specifiers[-1]
    
    # if there already exists a struct of name struct_name, nothing is added (false returned).
    # otherwise, the struct is added (true returned)
    # this checking is necessary since AST dumps some structs that 
    # seem to have the same name. 

    # e.g.,  '|-RecordDecl 0x23afe00 <line:67:9, line:72:1> line:67:9 union definition'
    # and '|-RecordDecl 0x23b0250 <line:75:9, line:80:1> line:75:9 union definition'
    # both have struct name 'union'. only 1 will be added (note that these structs
    # aren't useful for our analysis, but we don't know that until we've analyzed
    # the entire AST)
    def add_struct(self, struct_name: str) -> bool:
        for specifier in self.specifiers:
            if specifier.get_name() == struct_name: 
                return False
        
        new_specifier = Struct(struct_name)
        self.specifiers.append(new_specifier)
        return True 

def add_function(function_decl: str, specifier_manger: SpecifierManager) -> Specifier:
    # Assumption: functions that are extern are not relevant to our analysis 
    if function_decl.endswith("extern"):
        return

    quote_index = function_decl.find("'")
    start_index = quote_index - 2
    cur_char = function_decl[start_index]
    function_name = ""

    while (cur_char != " "):
        function_name = cur_char + function_name

        start_index -= 1
        cur_char = function_decl[start_index]
    
    start_index = quote_index + 1 
    cur_char = function_decl[start_index]
    return_type = ""

    while (cur_char != "("):
        return_type += cur_char

        start_index += 1 
        cur_char = function_decl[start_index]
    
    print(f"function name = '{function_name}', return type = '{return_type}'")
    return specifier_manger.add_function(function_name, return_type)

    # return type looks like
    # 'int '
    # 'void *'
    # 'struct my_struct '
    # 'unsigned short *' 
    # 'long long '

    # i think our rule can be 
    # (1) if it starts with a 'struct ', remove it, and (2) if the last char is a space, remove it. 
    # we might only need the 2nd rule 
    
def add_struct(record_decl: str, specifier_manger: SpecifierManager):
    record_decl_chunks = record_decl.split(" ")
    struct_name = record_decl_chunks[len(record_decl_chunks) - 2]

    if specifier_manger.add_struct(struct_name):
        print(f"adding struct '{struct_name}' from '{record_decl}'")

def add_parameter(parm_var_decl: str, param_index: int, func: Specifier):
    if type(func) is Struct:
        return
    
    print(f"adding param {parm_var_decl}")

    param_type = parm_var_decl[
        parm_var_decl.find("'")  + 1 : find_second_index(parm_var_decl, "'") 
    ]

    param = Parameter(param_index, param_type)
    func.add_parameter(param)

def add_field(field_decl: str, field_index: int, struct: Specifier):
    if type(struct) is Function:
        return

    start_index = field_decl.find("'") - 2
    cur_char = field_decl[start_index]
    field_name = ""

    while (cur_char != " "):
        field_name = cur_char + field_name

        start_index -= 1
        cur_char = field_decl[start_index]
    

    field = Field(field_name, field_index)
    struct.add_field(field)


# finds the index of the 2nd occurrence of char in string 
def find_second_index(string: str, char: str) -> int:
    return string.find(char, string.find(char) + 1)

# param_index, field_index are optional; can be either None or int 
def parse_anno(anno: str, spec: Specifier, annotation_manager: AnnotationManager, param_index: int = None, field_index: int = None):
    print(f"pre is {anno}")

    start_anno_index = anno.find('"')
    end_anno_index = anno.rfind('"')

    anno = anno[start_anno_index + 1 : end_anno_index]

    anno_type = anno[
        anno.find(" ") + 1 : find_second_index(anno, " ")
    ]

    anno_methods = anno[anno.rfind(" ") + 1::]

    known_target = ""

    anno_unfilled_target = ""
    anno_unfilled_target_index = find_second_index(anno, "_")

    while (anno[anno_unfilled_target_index] != " "):
        anno_unfilled_target += anno[anno_unfilled_target_index]
        anno_unfilled_target_index += 1
    
    # anno_unfilled_target looks like "_" or "_.FIELD(x)"

    if type(spec) is Struct:
        known_target = f"STRUCT({spec.get_name()}).FIELD({field_index})"
        

    elif type(spec) is Function:
        known_target = f"FUNCTION({spec.get_name()})"

        # Assumption: if it is not referring to a parameter, it is 
        # referring to the function itself 
        if param_index is None: 
            known_target += ".RETURN"
        else: 
            known_target += f".PARAM({param_index})"


    if ".FIELD" in anno_unfilled_target:
            anno_unfilled_target_field_name = anno_unfilled_target[
                anno_unfilled_target.find("(") + 1 : anno_unfilled_target.find(")")
            ]
            
            if type(spec) is Function:
                if param_index is None:
                    print(f"ret type is '{spec.return_type}'")
                    # Assumption: spec.return_type looks like 'struct <struct_name> '
                    return_struct_name = spec.return_type.split(" ")[1]
                    struct = specifier_manager.get_struct(return_struct_name)

                    for field in struct.fields:
                        if anno_unfilled_target_field_name == field.field_name:
                            anno_unfilled_target_field_name = field.field_index
                            break 
                    else:
                        raise ValueError(f"Did not find field '{anno_unfilled_target_field_name}' for struct '{struct.get_name()}', anno {anno}")
                else:
                    # for the parameter, we need the type (the struct that it is definitely referring to)
                    found_field = False

                    for param in spec.parameters:
                        if (found_field):
                            break 

                        if param.index == param_index:
                            # Assumption: param.param_type looks like 'struct <struct_name>'
                            param_type_struct_name = param.param_type.split(" ")[1]
                            struct = specifier_manager.get_struct(param_type_struct_name)

                            for field in struct.fields:
                                if anno_unfilled_target_field_name == field.field_name:
                                    anno_unfilled_target_field_name = field.field_index
                                    found_field = True 
                                    break 
                            else:
                                raise ValueError(f"Did not find field '{anno_unfilled_target_field_name}' for struct  '{struct.get_name()}', anno {anno}")
                            
                    if (not found_field):
                        raise ValueError(f"Did not find field '{anno_unfilled_target_field_name}' for anno '{anno}', specifier '{spec.get_name()}'")
                            
                
                print(f"UNFILLED_FIELD = {anno_unfilled_target_field_name}")
                known_target += f".FIELD({anno_unfilled_target_field_name})"

    
    print(f"for anno {anno} known target is {known_target} and {spec.get_name()}")
    built_anno = Annotation(anno_type, known_target, anno_methods)
    annotation_manager.add_annotation(built_anno)
    
file_to_read = sys.argv[1]
output_file = sys.argv[2]


with open(file_to_read) as ast:
    cur_top_decl = None  #FunctionDecl or RecordDecl
    cur_mid_decl = None #ParmVarDecl or FieldDecl
    param_index = None 
    field_index = None

    ast_lines = ast.readlines()
    specifier_manager = SpecifierManager() 
    annotation_manager = AnnotationManager()
    
    
    for (line, expr) in enumerate(ast_lines): 
        expr = expr.strip() 

        # gets decl 
        start_decl_index = expr.find(next(filter(str.isalpha, expr))) # finds index of first letter
        decl = ""
        cur_decl_char = expr[start_decl_index]

        while (cur_decl_char != " "):
            decl += cur_decl_char
            start_decl_index += 1
            cur_decl_char = expr[start_decl_index]

        if "FunctionDecl" == decl:
            cur_top_decl = expr
            param_index = None
            field_index = None 
            add_function(cur_top_decl, specifier_manager)
            
        elif "RecordDecl" == decl:
            cur_top_decl = expr
            param_index = None 
            field_index = None 
            add_struct(cur_top_decl, specifier_manager)

        elif "ParmVarDecl" == decl: 
            cur_mid_decl = expr
            field_index = None 

            if param_index is None: 
                param_index = 0
            else:
                param_index += 1
            add_parameter(cur_mid_decl, param_index, specifier_manager)


        elif "FieldDecl" == decl:
            cur_mid_decl = expr 
            param_index = None 
            
            if field_index is None: 
                field_index = 0
            else:
                field_index += 1

            add_field(cur_mid_decl, field_index, specifier_manager)
        
            
        elif "AnnotateAttr" == decl:
            annotation = expr

            if cur_mid_decl is None:
                param_index = None  

            # do parsing 
            print(f"top: {cur_top_decl}\nmid: {cur_mid_decl}\nanno: {annotation}\nparam_index: {param_index}\nfield_index: {field_index}\n")
            parse_anno(annotation, specifier_manager, annotation_manager, param_index, field_index)
            
            # if there's a "|-" on an annotation expression, we assume there is 
            # at least 1 more annotation
            # so "context" (cur_mid_decl) is not removed for the next line 
            if "|-" not in expr: 
                cur_mid_decl = None

        else:
            param_index = None
            field_index = None

    with open(output_file, "w") as testFile:
        for i in range(len(annotation_manager.annotations)):
            anno = annotation_manager.annotations[i]
            
            testFile.write(anno.to_str())

            if i != len(annotation_manager.annotations) - 1:
                testFile.write("\n")