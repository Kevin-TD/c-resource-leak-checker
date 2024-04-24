from enum import Enum
import re
import sys


class Tokens(Enum):
    FUNCTION = 1
    STRUCT = 2
    STRUCT_VARIBALE = 3
    ANNOTATION = 4

    NAME = 5
    RETURN_TYPE = 6
    PARAMETERS = 7
    FIELDS = 8
    TYPEDEFS = 9
    TYPE = 10
    ANNO_TYPE = 11
    TARGET = 12
    METHODS = 13

    START = 14 
    END = 15 

C_VAR_REGEX = r"\([a-zA-Z_][a-zA-Z0-9_]*\)"
STR_LIST_REGEX = "\[[^\[\]]*\]"

# TODO!: there are some parameters like this:
# @PARAMETERS [_IO_FILErestrict,constcharrestrict,struct __va_list_tag,intrestrict]
# with a space that make writing STR_LIST_REGEX not very specific in order to match it.
# need to investigate why there is space. for now, STR_LIST_REGEX matcher is looser.


class FormatSpecifier:
    def __init__(self):
        self.__token_path: "list[Tokens]" = [Tokens.START]
    
    def add_token(self, token: Tokens):
        if not type(token) is Tokens: 
            raise RuntimeError("Token is not of type Token")
        
        self.__token_path.append(token)

    def get_acceptable_next_token(self):
        if self.__token_path[-1] == Tokens.START:
            return [Tokens.END, Tokens.FUNCTION, Tokens.STRUCT, Tokens.STRUCT_VARIBALE, Tokens.ANNOTATION]
        
        elif self.__token_path[-1] == Tokens.FUNCTION:
            return [Tokens.NAME]

        elif self.__token_path[-1] == Tokens.STRUCT:
            return [Tokens.NAME]

        elif self.__token_path[-1] == Tokens.STRUCT_VARIBALE:
            return [Tokens.NAME]

        elif self.__token_path[-1] == Tokens.ANNOTATION:
            return [Tokens.ANNO_TYPE]
        
        elif self.__token_path[-1] == Tokens.NAME:
            if len(self.__token_path) < 2:
                raise RuntimeError("Token path should have at least 1 other token before checking for NAME token")
            
            # decl token should be FUNCTION, STRUCT, or STRUCT_VARIABLE.
            # we do not expect ANNOTATION as it has no NAME attribute token
            expected_decl_token = self.__token_path[-2]

            if expected_decl_token == Tokens.FUNCTION:
                return [Tokens.RETURN_TYPE]
            elif expected_decl_token == Tokens.STRUCT:
                return [Tokens.FIELDS]
            elif expected_decl_token == Tokens.STRUCT_VARIBALE:
                return [Tokens.TYPE]
            else:
                raise RuntimeError(f"Name following unexpected token {expected_decl_token}")
        
        elif self.__token_path[-1] == Tokens.RETURN_TYPE:
            return [Tokens.PARAMETERS]
        
        elif self.__token_path[-1] == Tokens.PARAMETERS:
            return [Tokens.END, Tokens.FUNCTION, Tokens.STRUCT, Tokens.STRUCT_VARIBALE, Tokens.ANNOTATION]
        
        elif self.__token_path[-1] == Tokens.FIELDS:
            return [Tokens.TYPEDEFS]
        
        elif self.__token_path[-1] == Tokens.TYPEDEFS:
            return [Tokens.END, Tokens.FUNCTION, Tokens.STRUCT, Tokens.STRUCT_VARIBALE, Tokens.ANNOTATION]
        
        elif self.__token_path[-1] == Tokens.TYPE:
            return [Tokens.END, Tokens.FUNCTION, Tokens.STRUCT, Tokens.STRUCT_VARIBALE, Tokens.ANNOTATION]
        
        elif self.__token_path[-1] == Tokens.ANNO_TYPE:
            return [Tokens.TARGET]
        
        elif self.__token_path[-1] == Tokens.TARGET:
            return [Tokens.METHODS]
        
        elif self.__token_path[-1] == Tokens.METHODS:
            return [Tokens.END, Tokens.FUNCTION, Tokens.STRUCT, Tokens.STRUCT_VARIBALE, Tokens.ANNOTATION]
        
        else:
            raise RuntimeError("Unrecognied token")


    def turn_token_str_to_token(self, token_str: str):
        if token_str == "*FUNCTION":
            return Tokens.FUNCTION

        elif token_str == "*STRUCT":
            return Tokens.STRUCT

        elif token_str == "*STRUCT_VARIABLE":
            return Tokens.STRUCT_VARIBALE

        elif token_str == "*ANNOTATION":
            return Tokens.ANNOTATION

        elif bool(re.search(f"@NAME {C_VAR_REGEX}", token_str)):
            return Tokens.NAME

        elif bool(re.search(f"@RETURN_TYPE \(.*?\)", token_str)):
            return Tokens.RETURN_TYPE

        elif bool(re.search(f"@PARAMETERS {STR_LIST_REGEX}", token_str)):
            return Tokens.PARAMETERS

        elif bool(re.search(f"@FIELDS {STR_LIST_REGEX}", token_str)):
            return Tokens.FIELDS

        elif bool(re.search(f"@TYPEDEFS {STR_LIST_REGEX}", token_str)):
            return Tokens.TYPEDEFS

        elif bool(re.search(f"@TYPE {C_VAR_REGEX}", token_str)):
            return Tokens.TYPE

        elif bool(re.search(f"@ANNO_TYPE {C_VAR_REGEX}", token_str)):
            return Tokens.ANNO_TYPE

        elif bool(re.search(f"@TARGET \<.*?\>", token_str)):
            return Tokens.TARGET

        elif bool(re.search(f"@METHODS {STR_LIST_REGEX}", token_str)):
            return Tokens.METHODS

        else:
            raise RuntimeError(
                f"Unable to convert string {token_str} into token")

