from abc import ABC as AbstractClass

class Specifier(AbstractClass):
    """Generic object that represents something an annotation can be placed on.
    Extended into Function and Struct
    """
    def __init__(self, name: str):
        self._name = name

    def get_name(self) -> str:
        return self._name