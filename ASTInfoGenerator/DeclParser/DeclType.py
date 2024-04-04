from abc import ABC as AbstractClass

class DeclType(AbstractClass):
    """Generic declaration type from of the AST
    """
    def __init__(self, decl_name: str):
        self._decl_name = decl_name

    def get_decl_name(self) -> str:
        return self._decl_name