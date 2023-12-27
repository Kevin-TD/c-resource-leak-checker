#! python3 annotation_generator.py <ast txt file> <output for annotations file> 
# generate AST text with: clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics <c file> > <ast txt file>

# examples (run in build dir): 
# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/test20.c > CodeAnalyzerFiles/test20_AST.txt ; python3 ../Annotations/annotation_generator.py CodeAnalyzerFiles/test20_AST.txt CodeAnalyzerFiles/test20_ANNOTATIONS.txt

# clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics ../test/test23/test24.c > CodeAnalyzerFiles/test24_AST.txt ; python3 ../Annotations/annotation_generator.py CodeAnalyzerFiles/test24_AST.txt CodeAnalyzerFiles/test24_ANNOTATIONS.txt


import sys 
from abc import ABC as AbstractClass

DEBUG = False

def logout(x: str):
    if (DEBUG):
        print(x)
    

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
        return f"{self.anno_type} target = {self.target} methods = {self.methods}"

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
    
    # if there already exists a function of name function_name, that function is returned.
    # otherwise, the function is added and returned. 
    def add_function(self, function_name: str, return_type: str) -> Specifier:
        for specifier in self.specifiers:
            if specifier.get_name() == function_name: 
                return specifier 
            
        new_specifier = Function(function_name, return_type)
        self.specifiers.append(new_specifier)
        return self.specifiers[-1]
    
    # if there already exists a struct of name struct_name, that struct is returned. 
    # otherwise, the struct is added and returned. 
    def add_struct(self, struct_name: str) -> bool:
        for specifier in self.specifiers:
            if specifier.get_name() == struct_name: 
                return specifier
        
        new_specifier = Struct(struct_name)
        self.specifiers.append(new_specifier)
        return self.specifiers[-1] 

def add_function(function_decl: str, specifier_manger: SpecifierManager) -> Specifier:
    # Assumption: functions that are extern are not relevant to our analysis 
    if function_decl.endswith("extern"):
        return None 

    logout(f"function decl equals {function_decl}")

    quote_index = function_decl.find("'")
    # Assumption: FunctionDecls wrap in quotes ('...') the return type of the function & paramters, and 
    # right before the return type is the name of the function. 
    # we also assume the function name and return type are separated by a space. 
    # e.g., |-FunctionDecl 0x24161a8 <../test/test23/test23.h:27:1, line:28:57> col:5 does_something 'struct my_struct (struct my_struct)'
    #                                                                                 ^func name     ^return type      ^paramters 

    start_index = quote_index - 2
    # quote_index - 2 since we will be traversing FunctionDecl 
    # backward, starting at the last character of the name of the function, 
    # in order to get the full function name

    cur_char = function_decl[start_index]
    function_name = ""

    while (cur_char != " "):
        function_name = cur_char + function_name

        start_index -= 1
        cur_char = function_decl[start_index]
    
    start_index = quote_index + 1 
    # quote_index + 1 since we assume this is where 
    # the return type is specified 

    cur_char = function_decl[start_index]
    return_type = ""

    while (cur_char != "("):
        # we assume that parameter types are specified within 
        # the open-close parentheses
        # e.g., 'int (int)' is a function that returns int with singular parameter int
        # e.g., 'int ()' is a function that returns int with no parameters

        # e.g., 'void (int, int, char *)' is a function that returns void with 
        # 3 parameters (in the order as it appears in C code), int, int, and char * 

        return_type += cur_char

        start_index += 1 
        cur_char = function_decl[start_index]
    
    logout(f"function name = '{function_name}', return type = '{return_type}'")
    return specifier_manger.add_function(function_name, return_type)
    
def add_struct(record_decl: str, specifier_manger: SpecifierManager):
    record_decl_chunks = record_decl.split(" ")
    struct_name = record_decl_chunks[len(record_decl_chunks) - 2]

    return specifier_manger.add_struct(struct_name)

def add_parameter(parm_var_decl: str, param_index: int, func: Specifier):
    if type(func) != Function:
        return
    
    logout(f"adding param {parm_var_decl}")

    param_type = parm_var_decl[
        parm_var_decl.find("'")  + 1 : find_second_index(parm_var_decl, "'") 
    ]

    param = Parameter(param_index, param_type)
    func.add_parameter(param)

def add_field(field_decl: str, field_index: int, struct: Specifier):
    if type(struct) != Struct: 
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
    logout(f"pre is {anno}")

    start_anno_index = anno.find('"')
    end_anno_index = anno.rfind('"')

    anno = anno[start_anno_index + 1 : end_anno_index]

    anno_type = anno[
        0 : anno.find(" ")
    ]

    anno_methods = anno[anno.rfind("methods = ") + len("methods = ")::]  
    logout(f"ANNO METHODS = {anno_methods}")

    known_target = ""

    anno_unfilled_target = ""
    anno_unfilled_target_index = anno.find("_")

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
                    logout(f"ret type is '{spec.return_type}'")
                    # Assumption: spec.return_type looks like 'struct <struct_name> ' or '<struct_name> ' 
                    # note: it'll look like '<struct_name> ' if a typedef is used 

                    # we can make this assumption because: 
                    # 1. param_index is none, which means we are making an 
                    # annotation for the function itself, and (for now) we assume
                    # that means it's an annotation on its return.
                    # note: we'll need to update how we treat annotations 
                    # applied to the function itself 
                    # if/when annotations like @Owning and @MustCallAlias are implemented
                    # 2. the ".FIELD" in anno_unfilled_target is specified by the user via 
                    # something like 
                    '''
                    char *MustCall("free") 
                        creates_obligation(char *s Calls("free"), struct my_struct X Calls("free", "x"));
                                                                                                   ^^^
                    '''
                    # the calling of that second parameter on Calls/MustCall tells us that for the struct 
                    # we are specifying, we are targeting some field. we verify whether that field 
                    # actually exists, since this pass does
                    # keep note of every struct and its fields. if we wrote something like:
                    # void Calls("free", "x") example(int S Calls("free", "x")) { ... }
                    #                                 ^^^                 ^^^ implying "int" has field "x"
                    # we'll throw "ValueError: Did not find field 'x' for struct 'int', anno Calls target = _.FIELD(x) methods = free"
                    
                    return_type_split = spec.return_type.split(" ")
                    if return_type_split[1] != "":
                        return_struct_name = return_type_split[1]
                    else:
                        return_struct_name = return_type_split[0]
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
                            # Assumption: param.param_type looks like 'struct <struct_name>' or '<struct_name>'
                            logout(f"param_type_struct_name = '{param.param_type}'")
                            try:
                                param_type_struct_name = param.param_type.split(" ")[1]
                            except IndexError:
                                param_type_struct_name = param.param_type


                            struct = specifier_manager.get_struct(param_type_struct_name)

                            for field in struct.fields:
                                if anno_unfilled_target_field_name == field.field_name:
                                    anno_unfilled_target_field_name = field.field_index
                                    found_field = True 
                                    break 
                            else:
                                raise ValueError(f"Did not find field '{anno_unfilled_target_field_name}' for struct '{struct.get_name()}', anno {anno}")
                            
                    if (not found_field):
                        raise ValueError(f"Did not find field '{anno_unfilled_target_field_name}' for anno '{anno}', specifier '{spec.get_name()}'")
                            
                
                logout(f"UNFILLED_FIELD = {anno_unfilled_target_field_name}")
                known_target += f".FIELD({anno_unfilled_target_field_name})"

    
    logout(f"for anno {anno} known target is {known_target} and {spec.get_name()}")
    built_anno = Annotation(anno_type, known_target, anno_methods)
    annotation_manager.add_annotation(built_anno)
    
file_to_read = sys.argv[1]
output_file = sys.argv[2]


with open(file_to_read) as ast:
    cur_spec = None #Function or Struct or None
    cur_top_decl = None  #FunctionDecl or RecordDecl (strings)
    cur_mid_decl = None #ParmVarDecl or FieldDecl (strings)
    param_index = None 
    field_index = None

    ast_lines = ast.readlines()
    specifier_manager = SpecifierManager() 
    annotation_manager = AnnotationManager()
    
    
    for expr in ast_lines: 
        expr = expr.strip() 
        logout(f"expr = {expr}")

        # gets decl 
        start_decl_index = expr.find(next(filter(str.isalpha, expr))) # finds index of first letter
        decl = ""
        cur_decl_char = expr[start_decl_index]

        try: 
            while (cur_decl_char != " "):
                decl += cur_decl_char
                start_decl_index += 1
                cur_decl_char = expr[start_decl_index]
        except IndexError:
            # might be a null stement (looks like "<<<NULL>>>" in AST)
            param_index = None
            field_index = None
            continue 

        if "FunctionDecl" == decl:
            cur_top_decl = expr
            param_index = None
            field_index = None 
            cur_spec = add_function(cur_top_decl, specifier_manager)
            
        elif "RecordDecl" == decl:
            cur_top_decl = expr
            param_index = None 
            field_index = None 
            cur_spec = add_struct(cur_top_decl, specifier_manager)

        elif "ParmVarDecl" == decl: 
            cur_mid_decl = expr
            field_index = None 

            if param_index is None: 
                param_index = 0
            else:
                param_index += 1
            add_parameter(cur_mid_decl, param_index, cur_spec)


        elif "FieldDecl" == decl:
            cur_mid_decl = expr 
            param_index = None 
            
            if field_index is None: 
                field_index = 0
            else:
                field_index += 1

            add_field(cur_mid_decl, field_index, cur_spec)
        
            
        elif "AnnotateAttr" == decl:
            annotation = expr

            if cur_mid_decl is None:
                param_index = None  

            logout(f"top: {cur_top_decl}\nmid: {cur_mid_decl}\nanno: {annotation}\nparam_index: {param_index}\nfield_index: {field_index}\n")
            parse_anno(annotation, cur_spec, annotation_manager, param_index, field_index)
            
            # if there's a "|-" on an annotation expression, we assume there is 
            # at least 1 more annotation,
            # so "context" (cur_mid_decl) is not removed for the next line 
            if "|-" not in expr: 
                cur_mid_decl = None

        else:
            param_index = None
            field_index = None

    with open(output_file, "w") as output:
        for i in range(len(annotation_manager.annotations)):
            anno = annotation_manager.annotations[i]
            
            output.write(anno.to_str())

            if i != len(annotation_manager.annotations) - 1:
                output.write("\n")