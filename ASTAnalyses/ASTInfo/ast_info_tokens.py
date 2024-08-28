# translation between token type and string representation in
# the ast info language
AST_INFO_TOKENS = {
    "DeclStart": "*",
    "FunctionDecl": "FUNCTION",
    "StructDecl": "STRUCT",
    "StructVarDecl": "STRUCT_VARIABLE",
    "AnnoDecl": "ANNOTATION",
    "LValueDecl": "LVALUE_DECLARATION",

    "NameAttr": "@NAME",
    "ReturnTypeAttr": "@RETURN_TYPE",
    "ParamsAttr": "@PARAMETERS",
    "FieldsAttr": "@FIELDS",
    "TypedefsAttr": "@TYPEDEFS",
    "TypeAttr": "@TYPE",
    "AnnoTypeAttr": "@ANNO_TYPE",
    "MethodsAttr": "@METHODS",
    "TargetAttr": "@TARGET",
    "LineNumberAttr": "@LINE_NUMBER",

    # separation between attr and the start of its declaration
    # e.g., for AttrValueSeparation = " ",
    # we expect "@NAME (name)"
    # e.g., for AttrValueSeparation = "|",
    # we expect "@NAME|(name)"
    "AttrValueSeparation": " ",

    "ValueOpenChar": "(",
    "ValueCloseChar": ")",
    "ValuesOpenChar": "[",
    "ValuesCloseChar": "]",
    "ValuesDelimeter": ",",
    "TargetOpenChar": "<",
    "TargetCloseChar": ">",

    "EndOfLineChar": "\n"
}

# convenience functions


def wrap_value_in_open_close(input_str: str) -> str:
    return AST_INFO_TOKENS['ValueOpenChar'] + input_str + AST_INFO_TOKENS['ValueCloseChar']


def wrap_value_list_in_open_close(input_str: str) -> str:
    return AST_INFO_TOKENS['ValuesOpenChar'] + input_str + AST_INFO_TOKENS['ValuesCloseChar']
