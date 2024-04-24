# *FUNCTION, *STRUCT, *STRUCT_VARIABLE, *ANNOTATION
# @NAME (string), @RETURN_TYPE (string), @PARAMETERS [list], @FIELDS [list], @TYPEDEFS [list], @TYPE (string), @ANNO_TYPE (string ), @TARGET <string>, @METHODS [list]

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


C_VAR_REGEX = r"\([a-zA-Z_][a-zA-Z0-9_]*\)"

STR_LIST_REGEX = "\[[^\[\]]*\]"

# TODO!: there are some parameters like this:
# @PARAMETERS [_IO_FILErestrict,constcharrestrict,struct __va_list_tag,intrestrict]
# with a space that make writing STR_LIST_REGEX not very specific in order to match it.
# need to investigate why there is space. for now, STR_LIST_REGEX matcher is looser.


class FormatSpecifier:
    def __init__(self):
        self.__token_path = []

    def get_acceptable_next_token(self, token: Tokens, token_path: "list[Tokens]"):
        if token == Tokens.FUNCTION:
            return [Tokens.NAME]

        elif token == Tokens.STRUCT:
            return [Tokens.NAME]

        elif token == Tokens.STRUCT_VARIBALE:
            return [Tokens.NAME]

        elif token == Tokens.ANNOTATION:
            return [Tokens.ANNO_TYPE]

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


r = FormatSpecifier()


# test code
file_to_read = sys.argv[1]
with open(file_to_read, "r") as f:
    for line in f:
        if line == "\n":
            continue

        l = line.strip()
        print(r.turn_token_str_to_token(l))
