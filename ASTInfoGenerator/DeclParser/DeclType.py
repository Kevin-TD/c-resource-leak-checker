from abc import ABC as AbstractClass

class DeclType(AbstractClass):
    """Generic declaration type from of the AST
    """
    def __init__(self, name: str):
        self._name = name

    def get_name(self) -> str:
        return self._name