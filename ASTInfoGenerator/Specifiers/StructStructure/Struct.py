from Specifiers.StructStructure.Field import Field
from Specifiers.Specifier import Specifier

class Struct(Specifier):
    """Represents a struct in the C code
    """
    def __init__(self, name: str):
        super().__init__(name)

        # typedefs are all alias names for the struct type that are defined with `typedef`keyword in C code
        self.__typedefs: list[str] = []
        self.__fields: list[Field] = []

    def get_typedefs(self) -> "list[str]":
        """returns copy of typedefs
        """
        return self.__typedefs.copy()

    def get_fields(self) -> "list[Field]":
        return self.__fields
    
    # returns true iff struct_name equals Struct's name or one of its typedefs
    def equals(self, struct_name: str) -> bool:
        return self._name == struct_name or struct_name in self.__typedefs

    def add_field(self, field: Field) -> None:
        self.__fields.append(field)

    def add_typedef(self, typedef: str) -> None:
        self.__typedefs.append(typedef)